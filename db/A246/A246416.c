/*
 * A246416  A permutation of essentially the duplicate nonnegative numbers:
 *          a(4n) = n + 1/2 - (-1)^n/2, a(2n+1) = a(4n+2) = 2n+1.
 * https://oeis.org/A246416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(4*n) = n if n even else n+1; a(2*n+1) = a(4*n+2) = 2*n+1
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd indices (and all 2*n+1 indices) */
    if (n & 1ULL)
        return (seq_t)n;

    /* indices congruent to 2 modulo 4: a(n)=n/2 */
    if ((n & 3ULL) == 2ULL)
        return (seq_t)(n >> 1);

    /* indices congruent to 0 modulo 4: a(n)=k + (k&1), where k=n/4 */
    idx_t k = n >> 2;                /* k = n/4 */
    return (seq_t)(k + (k & 1ULL));
}
