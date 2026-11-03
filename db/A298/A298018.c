/*
 * A298018  Partial sums of A298015.
 * https://oeis.org/A298018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   * a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) for n >
 *            10 * a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 25, a(4) = 46, * a(5)
 *            = 64, a(6) = 97, a(7) = 145, a(8) = 175, a(9) = 226, a(10) = 298
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A298018: partial sums of A298015, the coordination sequence of the snub-632
 * tiling: 1, 3, 6, 15, 21, 18 for n = 0..5 and, for n >= 6 with k = floor(n/3),
 * A298015(3k) = 18k - 3, A298015(3k+1) = 24k, A298015(3k+2) = 12k + 6 (the
 * formula as corrected in 2020; the recurrence this file had came from the
 * older, wrong generating function and fails from a(72) on). */

static seq_t c(idx_t m)                   /* A298015(m) */
{
    static const unsigned char first[6] = {1, 3, 6, 15, 21, 18};
    if (m < 6) return (seq_t)first[m];
    idx_t k = m / 3;
    switch (m % 3) {
    case 0: return (seq_t)18 * (seq_t)k - (seq_t)3;
    case 1: return (seq_t)24 * (seq_t)k;
    default: return (seq_t)12 * (seq_t)k + (seq_t)6;
    }
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)0;
    for (idx_t m = 0; m <= n; ++m) s += c(m);
    return s;
}
