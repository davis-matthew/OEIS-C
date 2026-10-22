/*
 * A389742  Number of integer compositions of n whose 0-prepended first
 *          differences are not all equal.
 * https://oeis.org/A389742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-1) - A007862(n) for n > 0, a(0)=0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007862(n) counts the number of triangular divisors of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute 2^(n-1) in seq_t */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow2 = pow2 * (seq_t)2;
    }

    /* Count k such that T_k = k*(k+1)/2 divides n */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t t;
        if ((k & 1) == 0) {
            t = (k / 2) * (k + 1);
        } else {
            t = k * ((k + 1) / 2);
        }
        if (t == 0 || t > n) break;
        if (n % t == 0) ++count;
    }

    return pow2 - (seq_t)count;
}
