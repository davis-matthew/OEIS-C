/*
 * A245541  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=(k-r)*(k-r+1)/2, or as n=2^k-2^r+j with 2 <= r <= k-1,
 *          1 <= j < 2^r-1, in which case a(2^k-2^r+j)=((k-r)*(k-r+1)/2)*a(j).
 * https://oeis.org/A245541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition:
   a(2^k - 2^r)  = (k - r)(k - r + 1)/2
   a(2^k - 2^r + j) = ((k - r)(k - r + 1)/2) * a(j),
   where 0 ≤ r ≤ k-1, 2 ≤ r ≤ k-1, 1 ≤ j < 2^r - 1.  */
seq_t a(idx_t n)
{
    /* Compute smallest power of 2 greater than n.  */
    idx_t msb = 63 - __builtin_clzll(n);          /* position of top bit of n */
    idx_t pow_k = (idx_t)1 << (msb + 1);          /* 2^(msb+1) > n */
    idx_t k = msb + 1;                            /* because 2^k = pow_k */

    idx_t diff = pow_k - n;                       /* diff = 2^k - n  */

    /* Find minimal r such that 2^r ≥ diff  (r < k). */
    idx_t r = 0;
    idx_t pow_r = 1;
    while (pow_r < diff) {
        pow_r <<= 1;
        ++r;
    }

    /* j = 2^r - diff;  j = 0 corresponds to the first case. */
    idx_t j = pow_r - diff;

    /* Triangular factor (k - r)(k - r + 1)/2 */
    seq_t tri = (seq_t)(k - r) * (seq_t)(k - r + 1) / 2;

    if (j == 0) {
        return tri;
    } else {
        return tri * a(j);
    }
}
