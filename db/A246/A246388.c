/*
 * A246388  Nonnegative integers k satisfying sin(k) >= 0 and sin(k+1) <= 0.
 * https://oeis.org/A246388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n + 1) * π)
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use the convergent 355/113 ≈ π, which is accurate enough for
       n up to at least 10^4 (error < 0.005 < 0.5). */
    unsigned __int128 k   = (unsigned __int128)n * 2 + 1; /* 2*n+1 */
    unsigned __int128 prod = k * 355;                    /* (2*n+1)*355 */
    return (seq_t)(prod / 113);                          /* floor((2*n+1)*π) */
}
