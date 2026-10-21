/*
 * A291162  Sequence of black numbers on the double zero (American) roulette
 *          wheel read counterclockwise.
 * https://oeis.org/A291162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The black numbers on the American double‑zero wheel appear in a fixed
   periodic order of 18 terms:
   2, 35, 4, 33, 6, 31, 8, 29, 10, 13, 24, 15, 22, 17, 20, 11, 26, 28.
   This list is repeated indefinitely counter‑clockwise.  The function
   below returns the n-th term for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Period of black numbers on the wheel */
    static const uint32_t wheel[18] = {
        2, 35, 4, 33, 6, 31, 8, 29,
        10, 13, 24, 15, 22, 17, 20, 11,
        26, 28
    };
    /* Index into the period: (n-1) modulo 18 */
    idx_t idx = (n - 1) % 18;
    return (seq_t)wheel[(unsigned)idx];
}
