/*
 * A378266  Number of subgroups of S_n isomorphic to S_3 up to conjugacy,
 *          where S_n is the n-th symmetric group.
 * https://oeis.org/A378266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = # of 4‑tuples (a,b,c,d) of non‑negative integers
 *            satisfying a + 2b + 3c + 6d = n and c + d ≥ 1. This counts the
 *            isomorphism classes of faithful S3‑sets of size n, i.e. the
 *            conjugacy classes of subgroups of the symmetric group S_n
 *            isomorphic to S3
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;               /* accumulator for the count          */
    /* iterate over the number of 6‑orbits (regular representations) */
    for (idx_t d = 0; d <= n / 6; ++d) {
        idx_t remaining = n - 6 * d;            /* points left after 6‑orbits     */
        /* c must be at least 1 if we have no 6‑orbit,
           otherwise c may be 0. */
        idx_t c_min = (d == 0) ? 1 : 0;
        /* maximum possible number of 3‑orbits */
        idx_t c_max = remaining / 3;
        for (idx_t c = c_min; c <= c_max; ++c) {
            /* after fixing c 3‑orbits, we solve a + 2b = remaining - 3c   */
            idx_t t = remaining - 3 * c;         /* non‑negative             */
            /* for each b from 0 to floor(t/2) we get a unique a   */
            seq_t add = (seq_t)(t / 2) + 1;      /* number of (a,b) pairs    */
            result += add;
        }
    }
    return result;
}
