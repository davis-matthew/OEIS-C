/*
 * A245227  Maximum frustration of complete bipartite graph K(n,5).
 * https://oeis.org/A245227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(25*n/16) for most n. If n==1 or n==3 or n ≡
 *            2,4,9,13,15 (mod 16) then subtract 1
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* compute floor(25*n/16) using integer arithmetic */
    seq_t val = (seq_t)25 * (seq_t)n / 16;

    /* apply the correction for the specified congruence classes */
    idx_t r = n % 16;
    if (n == 1 || n == 3 || r == 2 || r == 4 || r == 9 || r == 13 || r == 15)
        val -= 1;

    return val;
}
