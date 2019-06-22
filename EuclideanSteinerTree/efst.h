/***********************************************************************

	File:	efst.h
	Rev:	b-2
	Date:	02/28/2001

	Copyright (c) 1998, 2001 by Pawel Winter, Martin Zachariasen
				    & David M. Warme

************************************************************************

	Declarations for the Euclidean FST generator.

************************************************************************

	Modification Log:

	a-1:	12/11/98	martinz
		: Created.  Derived from Pawel and Martin's C++ program
			    using Warme's infrastructure.
	b-1:	01/22/00	martinz
		: Dynamic allocation of eq-point array using doubling.
		:  (-e option is now the INITIAL number of eq-points
		:  per terminal.)
		: Added epsilon variable for floating point comparisons.
		: Added mean of points.  Translating instance so mean
		:  is at origin maximizes eq-point precision.
		: Added bsd parm to upper bound heuristic.
		: Added new greedy heuristic.
	b-2:	02/28/2001	warme
		: Use GMP, if available, to compute eq-points and
		:  EFST lengths precisely.

************************************************************************/

#ifndef EFST_H
#define EFST_H

#include "bsd.h"
#include "config.h"
#include "egmp.h"
#include "steiner.h"

/*
 * A structure to keep track of one EFST.  They are kept in a hash table
 * so that we can rapidly identify duplicates.	We also keep them all in
 * one big list, which is doubly-linked so that we can rapidly delete a
 * duplicate that is suboptimal.
 */

struct elist {
	struct elist *		forw;	/* Next EFST in saved list */
	struct elist *		back;	/* Previous EFST in saved list */
	struct elist *		next;	/* Next EFST in hash table */
	int			size;	/* Number of terminals */
	struct full_set *	fst;	/* The actual FST */
};

/*
 * Type used to store the terminal lists.
 */

typedef	int16u			eterm_t;


/*
 * The current state for a single equilateral point.
 */

struct eqp_t {
	struct point	E;	/* Coordinates of equilateral point */
  	struct point 	DV;     /* Displacement vector (relative to terminal) */
	struct point	LP;	/* Left endpoint of Steiner arc */
	struct point	RP;	/* Right endpoint of Steiner arc */
	struct eqp_t *	L;	/* Pointer to left eq-point */
	struct eqp_t *	R;	/* Pointer to right eq-point */
	int		S;	/* Number of terminals used in construction */
	dist_t		UB;	/* Upper bound on tree spanning Steiner */
				/* arc and terminals */
        dist_t		MDIST;
	dist_t		BS;	/* Shortest BSD between terminals */
	struct point	DC;	/* Center of eq-point circle */
	dist_t		DR;	/* Radius of eq-point circle */
	dist_t		DR2;	/* Squared radius of eq-point circle */
	eterm_t *	Z;	/* Pointer to list of terminals */
	int		SMINX;	/* Range of eq-point recangle in */
	int		SMAXX;	/* square data structure */
	int		SMINY;
	int		SMAXY;
	boolType		CHOSEN;
	int used;
};


/*
 * Equilateral point rectangle data structure
 */

struct eqp_square_t {
	struct eqp_t ** eqp;	/* Pointer to array of eq-points */
	int		n;	/* Number of eq-points in array */
};

/*
 * Global information used by the EFST generator.
 */

struct einfo {
             /*The set of terminals*/
        int**  matrix;
	int**  bsdmatrix;
	struct pset *	pts;		/* The set of terminals */
	int *		x_order;	/* Order of terminals by X, Y, index */
					/* directions (plus 3 wrap dirs) */
	dist_t		mst_length;	/* Length of the MST */
	dist_t		max_mst_edge;	/* Length of longest MST edge */
	struct bsd *	bsd;		/* Bottleneck Steiner distance info */
	dist_t		eps;		/* Relative epsilon used for comparisons */
	struct point	mean;		/* Mean point of all terminals */
	struct eqp_t *	eqp;		/* Array of equilateral points */
	int		eqp_size;	/* Current size of eq-point array */
	int *		size_start;	/* Starting index of eq-points of */
					/* given size */
	int		zalloc_size;	/* Current maximum number of */
					/* terminal lists */
	eterm_t *	eqpZ;		/* List of terminals for each eq-point */
	int		eqpZ_size;	/* Current size of terminals list */
	eterm_t *	eqpZ_curr;	/* Current allocation pointer */
	boolType *		MEMB;		/* For checking eq-point overlap */

	/* Variables used while generating eq-points */
	dist_t		dxi, dyi, dxj, dyj;
	struct pset *	termlist;

	/* Variables used for storing eq-point rectangles */
	dist_t		eqp_square_size;	/* Size of squares */
	struct eqp_square_t **	eqp_squares;	/* Eq-point squares */
	dist_t		minx, maxx, miny, maxy; /* Terminal coordinate range */
	int		srangex, srangey;	/* Range of squares */

	int		fsts_checked;	/* Num FSTs sent to screening tests */
	boolType *		term_check;	/* To compare FST terminal sets */

	int		num_term_masks; /* Size of terminal mask in each FST */

	struct elist ** hash;		/* FST hash table */
	struct elist	list;		/* Head of circular EFST list */

	int		ntrees;		/* Final number of FSTs */
	struct full_set * full_sets;	/* Final list of FSTs */
	struct full_set ** hookp;	/* For adding to end of FST list */

#ifdef HAVE_GMP
	struct qr3_point cur_eqp;	/* Exact pos of current eq-point */
#endif
};

/*
 * Function Prototypes.
 */

extern dist_t		smith_lee_liebman (struct pset *, struct bsd *);
extern dist_t		greedy_heuristic  (struct pset *, struct bsd *);
#ifdef __cplusplus
extern "C" {
#endif 
struct cinfo* buildSteinerTrees (char *);
#ifdef __cplusplus
}
#endif 

#endif
