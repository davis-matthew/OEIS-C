/*
 * A370055  a(n) = 3*(3*n+2)!/(2*n+3)!.
 * https://oeis.org/A370055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+2)!/(2*n+3)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, a(0) = 1  (the empty product is taken as 1) */
/* For n = 1, a(1) = 3 (the empty product again). */
/* For n >= 2, a(n) = 3*∏_{k=2n+4}^{3n+2} k.  */

seq_t a(idx_t n)
{
    /* Handle the two special cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Compute the product for n >= 2. */
    seq_t result = (seq_t)3;           /* the leading factor 3 */
    idx_t start = 2*n + 4;             /* first factor of the product */
    idx_t end   = 3*n + 2;             /* last factor of the product */

    for (idx_t k = start; k <= end; ++k)
        result *= (seq_t)k;            /* multiply the current factor */

    return result;
}
