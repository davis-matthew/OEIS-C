/*
 * A332061  Number of iterations of z -> z^2 + 1/4 + 1/n until z >= 2,
 *          starting with z = 0.
 * https://oeis.org/A332061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332061: returns the number of iterations of z → z² + 1/4 + 1/n
   starting from z = 0 until z ≥ 2.  n ≥ 1. */

seq_t a(idx_t n)
{
    /* c = 1/4 + 1/n = (n+4) / (4n)  */
    long double c = ((long double)(n + 4)) / (4.0L * (long double)n);
    long double z = 0.0L;
    seq_t iter = 0;
    while (z < 2.0L) {
        z = z * z + c;
        ++iter;
    }
    return iter;
}
