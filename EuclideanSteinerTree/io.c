/***********************************************************************

	File:	io.c
	Rev:	b-1
	Date:	02/28/2001

	Copyright (c) 1993, 2001 by David M. Warme

************************************************************************

	Routines for Input/Output of point sets and coordinates.
	These routines insulate the rest of the system from the
	exact data type for "coord_t".

	To eliminate errors due to the basic properties of binary
	floating point representation, we scale (i.e., multiply)
	every coordinate by a power of 10 until every coordinate has
	an integral value.  This is done PRIOR to conversion into
	floating point form, and permits us to represent each
	coordinate (as well as delta X and delta Y values) exactly.
	This actually eliminates all numeric errors when computing
	and summing distances in the rectilinear metric (as long as
	the numbers do not exceed the basic precision of a double).

************************************************************************

	Modification Log:

	a-1:	02/20/93	warme
		: Created.
	b-1:	02/28/2001	warme
		: Changes for 3.1 release.
		: Moved Data_Num_Decimal_Places and min_precision
		:  into new struct scale_info.  Now passing this
		:  scale_info explicitly everywhere it is needed.
		: Changed "struct numlist" to be a partially converted
		:  scaled numeric form rather than textual form.
		: Accept scientific notation and leading + signs.
		: Support negative scale factors.

************************************************************************/

#include "steiner.h"
#include "bsd.h"
#include "efst.h"


/*
 * Global Routines
 */

int			compute_scaling_factor (struct numlist *);
void			coord_to_string (char *, coord_t, struct scale_info *);
void			dist_to_string (char *, dist_t, struct scale_info *);
struct pset *		get_points (FILE *, struct scale_info *);
void                    get_efstInFile (FILE *	fp, struct scale_info *,struct pset **,int ***,int ***);
void			init_output_conversion (struct pset *,
						int,
						struct scale_info *);
struct numlist *	parse_line_of_numbers (FILE *);
coord_t			read_numlist (struct numlist *, struct scale_info *);
void			set_scale_info (struct scale_info *, int);


/*
 * Local Routines
 */

static struct numlist *	new_numlist (double, int, int);
static int		next_char (FILE *);
static struct numlist *	parse_input_numbers (FILE *);
static struct numlist *	parse_one_number (FILE *, boolType);

/*
 * This routine reads in a set of points from the standard input.
 */


/*
 * This routine reads in a set of polygons from the standard input.
 */

void
get_efstInFile (

FILE *			fp,		/* IN - stream to read from */
struct scale_info *	sip,		/* OUT - problem scaling info */
struct pset **outPoints2,
int ***matrix,
int ***bsdmat
)
{
struct bsd *outbsd;
int** bottleneckdist;
int			i,j;
int			n;
int                     npoly;
int                     numpolys;
int			scaling_factor;
struct pset *	points;

struct point *	p;
int			x;
int			y;
struct numlist *	list;
struct numlist *	nlp;
int                     polygonNumbers[100]; // max 100 polygons
int numPoints;
int type;
int polynum;
int used;
struct scale_info 	sip2;
	// Gather all numbers from the input in an accurate scaled form. 
       
      
	//list = parse_input_numbers (fp);

	// Determine the coordinate scaling factor to use.		
	//scaling_factor = compute_scaling_factor (list);

	//set_scale_info (sip, scaling_factor);

        sip->scale = 1;
        nlp = list;
          sip2.scale = 0;
        fscanf(fp,"%d",&numPoints);// read_numlist (nlp, &sip2);
        
        //nlp = nlp->next;
      
        //numpolys = read_numlist (nlp, &sip2);
        fscanf(fp,"%d",&numpolys);

        points = NEW_PSET (numPoints);                
        ZERO_PSET (points, numPoints);
        points->n = numPoints;
              
        int auxPolygon = -1;
        p = &(points->a[0]);
        //read points

	for (i=0;i< numPoints;i++) {
             // nlp = nlp->next;
             // x = read_numlist (nlp, sip);
              //nlp = nlp->next;
              //y = read_numlist (nlp, sip);
              //nlp = nlp->next;
             // polynum = read_numlist (nlp, &sip2);
              fscanf(fp,"%d %d %d %d",&x,&y,&polynum,&used);
              //fscanf(fp,"%f",&y);
              //fscanf(fp,"%d",&polynum);

              p->x = x/sip->scale;
              p->y = y/sip->scale;
              p->pnum = i;
              p->polygon = polynum;
	      p->used = used;
              if(auxPolygon < 0)
                if(polynum >= 0)
                {                    
                  points->polygonInit = i;
                  auxPolygon = 1;
                }
              
              p++;           
	}
        
         if(numpolys == 1)
		points->polygonInit =  numPoints -1;
        points->polygonEnd = numPoints -1;
        *outPoints2 = points;
        
        (*matrix) =  malloc ( sizeof(int*)*numPoints );
        for(i=0; i < numPoints;i++)
             (*matrix)[i] =  malloc ( sizeof(int)*numPoints );
        
        for (i=0;i< numPoints;i++)           
            for (j=0;j< numPoints;j++) {
                //nlp = nlp->next;
                 fscanf(fp," %d",&(*matrix)[i][j]);
                 //printf("%d ",(*matrix)[i][j]);
                }


	(*bsdmat) =  malloc ( sizeof(int*)*numPoints );
        for(i=0; i < numPoints;i++)
             (*bsdmat)[i] =  malloc ( sizeof(int)*numPoints );
        
        for (i=0;i< numPoints;i++)           
            for (j=0;j< numPoints;j++) {
                //nlp = nlp->next;
                 fscanf(fp," %d",&(*bsdmat)[i][j]);
		 (*bsdmat)[i][j] /= sip->scale;
                 printf("%d ",(*bsdmat)[i][j]);
                }
  
        /*outbsd = NEW(struct bsd);
        outbsd->mst_edges = NULL;
        outbsd -> ematrix = NEWA (numPoints * (numPoints), int16u);
        outbsd -> n=numPoints;
        outbsd -> method = 1;*/
                // Fill in the matrix, one row at a time...
 /*
        int k = 0;
		for (i = 0; i < numPoints; i++) {
			for (j = 0; j < numPoints; j++) {

				 [i] = bottleneckdist[i][j];
                              //   printf("%d ", bottleneckdist[i][j] );
                                 k++;
			}
		}
  
        
        *outbsd2 = outbsd;
        for(i = 0;i<numPoints;i++ )
              free (bottleneckdist[i]);
        free (bottleneckdist);
        */
     

}



	struct pset *
get_points (

FILE *			fp,		/* IN - stream to read from */
struct scale_info *	sip		/* OUT - problem scaling info */
)
{
int			i;
int			n;
int			scaling_factor;
struct pset *		pts;
struct point *		p;
coord_t			x;
coord_t			y;
struct numlist *	list;
struct numlist *	nlp;

	/* Gather all numbers from the input in an accurate scaled form. */

	list = parse_input_numbers (fp);

	/* Determine the coordinate scaling factor to use.		*/
	scaling_factor = compute_scaling_factor (list);

	set_scale_info (sip, scaling_factor);

	n = 0;
	for (nlp = list; nlp NE NULL; nlp = nlp -> next) {
		++n;
	}

	if ((n & 1) NE 0) {
		(void) fprintf (stderr, "X coord without Y coord.\n");
		exit (1);
	}
	n >>= 1;

	pts = NEW_PSET (n);

	/* Zero out the entire point set. */
	ZERO_PSET (pts, n);

	pts -> n = n;

	n = 0;
	p = &(pts -> a [0]);

	while (list NE NULL) {
		nlp = list;
		list = list -> next;

		x = read_numlist (nlp, sip);

		if (list EQ NULL) {
			fatal ("get_points: Bug 1.");
		}

		nlp = list;
		list = list -> next;

		y = read_numlist (nlp, sip);

		p -> x		= x;
		p -> y		= y;
		p -> pnum	= ((pnum_t) n);

		++n;
		++p;
	}

	if (n NE pts -> n) {
		fatal ("get_points: Bug 2.");
	}

	return (pts);
}

/*
 * This routine "parses" all of the numbers in the input into a list
 * of partially converted/scaled numbers in binary (floating point) form.
 */

	static
	struct numlist *
parse_input_numbers (

FILE *		fp		/* IN - input stream to read from */
)
{
struct numlist *	p;
struct numlist *	root;
struct numlist **	hookp;

	root = NULL;
	hookp = &root;

	for (;;) {
		p = parse_one_number (fp, FALSE);
		if (p EQ NULL) break;

		*hookp = p;
		hookp = &(p -> next);
	}

	return (root);
}

/*
 * Read a single text line of numbers.  We ignore blank lines and
 * lines containing only a comment.  Otherwise, this routine collects
 * numbers in a linked list of numlist structures until the first
 * newline after at least one number is read.
 */

	struct numlist *
parse_line_of_numbers (

FILE *		fp		/* IN - file to read from */
)
{
struct numlist *	root;
struct numlist **	hookp;
struct numlist *	p;

	root = NULL;
	hookp = &root;

	for (;;) {
		p = parse_one_number (fp, TRUE);
		if (p NE NULL) {
			*hookp = p;
			hookp = &(p -> next);
			continue;
		}

		/* We read either EOF or a newline... */
		if (root NE NULL) {
			/* we have one or more numbers -- we don't	*/
			/* really care whether it was newline or EOF!	*/
			break;
		}
		if (feof (fp)) {
			/* It was EOF.  We are not going to get any	*/
			/* more numbers!				*/
			break;
		}
		/* ignore blank/comment line. */
	}

	return (root);
}

/*
 * This routine "parses" the next number from the input stream, and converts
 * it into binary form.  This form is "integerized" to minimize numeric
 * error caused by fractional digits.
 */

	static
	struct numlist *
parse_one_number (

FILE *		fp,		/* IN - input stream to read from */
boolType		find_newline	/* IN - fail if newline seen before number */
)
{
int		num_dp;
int		ndig;
int		nsig;
int		nzero;
int		expon;
int		esign;
boolType		dot_flag;
int		c;
char *		p;
double		sign;
double		num;
double		fnum;
double		fpow;

#define MAX_INT_DIGITS	30

	if (feof (fp)) return (NULL);

	sign = 1.0;

	do {
		c = next_char (fp);
		if (c < 0) return (NULL);
		if (c EQ '.') break;
		if (c EQ '-') {
			/* Note the minus sign and proceed. */
			sign = -1.0;
			c = next_char (fp);
			break;
		}
		if (c EQ '+') {
			c = next_char (fp);
			break;
		}
		if (ispunct (c)) {
			/* Strip comment line... */
			do {
				c = next_char (fp);
				if (c < 0) return (NULL);
			} while (c NE '\n');
			if (find_newline) return (NULL);
			continue;
		}
		if ((c EQ '\n') AND find_newline) return (NULL);
	} while (NOT isdigit (c));

	num = 0.0;
	fnum = 0.0;
	fpow = 1.0;

	ndig = 0;
	nsig = 0;
	nzero = 0;
	num_dp = 0;
	dot_flag = FALSE;

	for (;;) {
		if (c EQ '.') {
			if (dot_flag) {
				/* OOPS!  More than 1 decimal point	*/
				/* seen in a single number...  Just	*/
				/* terminate this number.		*/
				ungetc (c, fp);
				break;
			}
			dot_flag = TRUE;
		}
		else if (c EQ '0') {
			if (dot_flag) {
				++num_dp;
			}
			if (nsig <= 0) {
				/* Ignore: no non-zero digits yet seen. */
			}
			else {
				/* Don't process these if we don't have to. */
				++nzero;
			}
		}
		else if (isdigit (c)) {
			/* Process any deferred zero digits... */
			while (nzero > 0) {
				if (nsig < MAX_INT_DIGITS) {
					num *= 10.0;
					++nsig;
				}
				else {
					fpow *= 10.0;
					if (dot_flag) {
						--num_dp;
					}
				}
				--nzero;
			}
			if (nsig < MAX_INT_DIGITS) {
				num = 10.0 * num + (c - '0');
				++nsig;
				if (dot_flag) {
					++num_dp;
				}
			}
			else {
				fpow *= 10.0;
				fnum += (c - '0') / fpow;
			}
		}

		c = next_char (fp);
		if (c < 0) break;

		if ((NOT isdigit (c)) AND (c NE '.')) break;
	}

	expon = 0;

	if ((c EQ 'e') OR (c EQ 'E')) {
		/* Read the exponent. */
		esign = 1;
		c = next_char (fp);
		if (c EQ '+') {
			c = next_char (fp);
		}
		else if (c EQ '-') {
			esign = -1;
			c = next_char (fp);
		}
		while ((c >= 0) AND isdigit (c)) {
			expon = 10 * expon + (c - '0');
			c = next_char (fp);
		}
		expon *= esign;
	}
	if (c >= 0) {
		ungetc (c, fp);
	}

	/* Put sign, integral, and fractional parts together. */
	num = sign * (num + fnum);

	expon += nzero;
	expon -= num_dp;

	return (new_numlist (num, expon, nsig));
}

/*
 * This routine creates a scaled number list structure.
 */

	static
	struct numlist *
new_numlist (

double		mantissa,	/* IN - scaled mantissa value */
int		expon,		/* IN - exponent (power of 10) */
int		nsig		/* IN - num significant mantissa digits */
)
{
struct numlist *	p;

	p = NEW (struct numlist);

	p -> mantissa	= mantissa;
	p -> expon	= expon;
	p -> nsig	= (nsig > 0) ? nsig : 1;
	p -> next	= NULL;

	return (p);
}

/*
 * Compute a common scale value for all of the numbers in the given list.
 * In most cases, we can obtain an "optimum" scale value -- wherein every
 * number in the list will have an integer value that can be represented
 * exactly.  In cases where there are many significant digits, or a wide
 * span of exponent values, we must compromise.
 */

	int
compute_scaling_factor (

struct numlist *	p	/* IN - list of numbers to scale */
)
{
int		k;
int		min_expon;
int		max_expon;

	for (;;) {
		if (p EQ NULL) {
			/* No non-zero numbers -- don't scale at all! */
			return (0);
		}
		if (p -> mantissa NE 0.0) break;
		p = p -> next;
	}

	min_expon = p -> expon;
	max_expon = p -> expon + p -> nsig;

	for (p = p -> next; p NE NULL; p = p -> next) {
		if (p -> mantissa EQ 0) continue;
		if (p -> expon < min_expon) {
			min_expon = p -> expon;
		}
		k = p -> expon + p -> nsig;
		if (k > max_expon) {
			max_expon = k;
		}
	}

	/* Let D be a significant digit from the list, E be its		*/
	/* exponent so that the digit has value D * 10**E.  We now know	*/
	/* the smallest and largest of the E values.  If the spread is	*/
	/* MAX_INT_DIGITS or fewer, we win!  (Use the smallest E.)	*/
	/* Otherwise, try to split the difference.			*/

	max_expon -= MAX_INT_DIGITS;

	if (max_expon > min_expon) {
		/* Must compromise. */
		min_expon = (max_expon + min_expon + 1) / 2;
	}

	/* Our internal representation is such that dividing it by	*/
	/* 10**-min_expon yields external values.  (Note that min_expon	*/
	/* is typically negative here, in which case we would really be	*/
	/* multiplying by 10**(-min_expon).)				*/

	return (-min_expon);
}

/*
 * Set the scaling info properly for the given scale factor.
 */

	void
set_scale_info (

struct scale_info *	sip,		/* OUT - scale_info to initialize */
int			scale_factor	/* IN - scale factor to establish */
)
{
int		i;
double		pow10;
double		p;

	memset (sip, 0, sizeof (*sip));

	sip -> scale	= scale_factor;

	i = scale_factor;
	if (i < 0) {
		i = - i;
	}

	/* Compute p = 10 ** i. */
	pow10 = 1.0;
	p = 10.0;
	while (i > 0) {
		if ((i & 1) NE 0) {
			pow10 *= p;
		}
		p *= p;
		i >>= 1;
	}

	if (scale_factor < 0) {
		sip -> scale_mul = pow10;
		sip -> scale_div = 1.0;
	}
	else {
		sip -> scale_mul = 1.0;
		sip -> scale_div = pow10;
	}
}

/*
 * This routine converts the coordinate in the given numlist structure
 * to an internal coordinate value, scaled as specified by the scaling_factor
 * parameter.  The given numlist structure is freed.
 */

	coord_t
read_numlist (

struct numlist *	nlp,		/* IN - string list structure */
struct scale_info *	sip		/* IN - problem scaling info */
)
{
int		i;
int		target_expon;
int		expon;
coord_t		value;
double		pow10;
double		p;

	target_expon = - sip -> scale;

	value = nlp -> mantissa;
	expon = nlp -> expon;

	i = expon - target_expon;
	if (i < 0) {
		i = - i;
	}
	pow10 = 1.0;
	p = 10.0;
	while (i > 0) {
		if ((i & 1) NE 0) {
			pow10 *= p;
		}
		p *= p;
		i >>= 1;
	}
	if (expon > target_expon) {
		value *= pow10;
	}
	else if (expon < target_expon) {
		value /= pow10;
	}

	free ((char *) nlp);

	return (value);
}

/*
 * This routine gets the next character from stdin.  For the case of
 * interactive input, it makes sure that an EOF condition is
 * "permanent" -- that is, getc will not be called again after it
 * indicates EOF.
 */

	static
	int
next_char (

FILE *		fp		/* IN - input stream to read from */
)
{
int		c;

	if (feof (fp)) {
		/* Don't do "getc" again -- EOF is permanent! */
		return (EOF);
	}

	c = getc (fp);

	return (c);
}

/*
 * This routine converts the given internal-form coordinate to a
 * printable ASCII string in external form.  The internal form is an
 * integer (to eliminate numeric problems), but the external data
 * may actually involve decimal fractions.  This routine therefore
 * properly scales the coordinate during conversion to printable form.
 */

	void
coord_to_string (

char *			buf,	/* OUT - buffer to put ASCII text into */
coord_t			coord,	/* IN - coordinate to convert */
struct scale_info *	sip	/* IN - problem scaling info */
)
{
	/* Just pretend its a distance -- distances certainly	*/
	/* do not have LESS precision than coordinates...	*/
	dist_to_string (buf, coord, sip);
}

/*
 * This routine converts the given internal-form distance to a
 * printable ASCII string in external form.  The internal form is an
 * integer (to eliminate numeric problems), but the external data
 * may actually involve decimal fractions.  This routine therefore
 * properly scales the distance during conversion to printable form.
 */

	void
dist_to_string (

char *			buf,	/* OUT - buffer to put ASCII text into */
dist_t			dist,	/* IN - distance to convert */
struct scale_info *	sip	/* IN - problem scaling info */
)
{
int		i;
int		digit;
int		mindig;
double		ipart;
double		fpart;
double		div10;
char *		p;
char *		endp;
char		tmp [256];

	if (dist < 0.0) {
		dist = -dist;
		*buf++ = '-';
	}
	if (dist EQ 0) {
		*buf++ = '0';
		*buf++ = '\0';
		return;
	}

	mindig = sip -> min_precision;

	memset (tmp, '0', sizeof (tmp));
	p = &tmp [128];
	endp = p;

	ipart = floor (dist);
	while (ipart > 0) {
		div10 = floor (ipart / 10.0);
		digit = ipart - floor (10.0 * div10 + 0.5);
		*--p = digit + '0';
		ipart = div10;
	}
	if ((sip -> scale <= 0) AND (mindig EQ 0)) {
		/* The coordinates and edge costs are all integers	*/
		/* (both internally and externally so scaling is a	*/
		/* "no-op").  The metric is not Euclidean, so nothing	*/
		/* irrational appears.  Always output integers, and do	*/
		/* NOT insert a decimal point...			*/

		endp [-(sip -> scale)] = '\0';
		strcpy (buf, p);
		return;
	}

	if (sip -> scale >= 0) {
		if (p + sip -> scale > endp) {
			p = endp - sip -> scale;
		}

		for (i = 0; i <= sip -> scale; i++) {
			endp [1 - i] = endp [0 - i];
		}
		endp [1 - i] = '.';
		++endp;

		fpart = dist - floor (dist);
		while ((endp - p) < (mindig + 1)) {
			fpart *= 10.0;
			digit = (int) floor (fpart);
			*endp++ = digit + '0';
			fpart -= ((double) digit);
		}
	}
	else if (mindig EQ 0) {
		endp -= sip -> scale;
	}
	else {
		fpart = dist - floor (dist);
		for (i = sip -> scale; i < 0; i++) {
			fpart *= 10.0;
			digit = (int) floor (fpart);
			*endp++ = digit + '0';
			fpart -= ((double) digit);
			--mindig;
		}
		if (mindig > 0) {
			*endp++ = '.';
			while (mindig > 0) {
				fpart *= 10.0;
				digit = (int) floor (fpart);
				*endp++ = digit + '0';
				fpart -= ((double) digit);
				--mindig;
			}
		}
	}

	*endp = '\0';

	strcpy (buf, p);
}

/*
 * This routine sets up the various parameters needed for outputting
 * scaled coordinates.  We want to print coordinates/distances with
 * the minimum fixed precision whenever this gives the exact result.
 * Otherwise we will print the coordinates/distances with full
 * precision.
 *
 * The minimum fixed precision is exact ONLY when ALL of the following
 * are true:
 *
 *	- The metric must not be EUCLIDEAN, since distances become
 *	  irrational even if the coordinates are finite precision.
 *	  Coordinates of Euclidean Steiner points are also irrational.
 *
 *	- The SCALED vertex coordinates must all be integral.
 *
 *	- The SCALED hyperedge costs must all be integral.
 *
 * Note: we actually check the scaled data for integrality because there
 * are some old FST generators that do not implement scaling!  Such
 * data always contain a scale factor of zero and non-integral coordinates
 * and distances.
 */

	void
init_output_conversion (

struct pset *		pts,		/* IN - list of terminals */
int			metric,		/* IN - the metric */
struct scale_info *	sip		/* IN/OUT - problem scaling info */
)
{
int			i;
struct point *		p;
boolType			integral;
double			c;

	integral = TRUE;

	if (pts NE NULL) {
		p = &(pts -> a [0]);
		for (i = 0; i < pts -> n; i++, p++) {
			c = floor ((double) (p -> x));
			if (c NE p -> x) {
				integral = FALSE;
				break;
			}
			c = floor ((double) (p -> y));
			if (c NE p -> y) {
				integral = FALSE;
				break;
			}
		}
	}

	if (integral AND (metric NE EUCLIDEAN)) {
		sip -> min_precision = 0;
	}
	else {
		sip -> min_precision = DBL_DIG + 1;
	}
}
