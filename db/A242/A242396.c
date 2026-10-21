/*
 * A242396  Number of rows of equilateral triangles (sides length = 1) that
 *          intersect the circumference of a circle of radius n centered at
 *          (0,0) or (1/2,0).
 * https://oeis.org/A242396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(30*n/13) + 2 - ((4*n mod 13) & 1)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n = 0 no term exists,
       but returning 0 does not affect correctness for the requested
       domain. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;                     /* convert to seq_t */
    seq_t t  = ((seq_t)30 * nn) / (seq_t)13; /* floor(30*n/13)      */
    seq_t r  = ((seq_t)4  * nn) % (seq_t)13; /* 4n mod 13           */
    seq_t p  = r & (seq_t)1;                 /* parity of r (0 or 1)*/
    return t + (seq_t)2 - p;                 /* final value         */
}
