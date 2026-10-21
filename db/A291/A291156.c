/*
 * A291156  Sequence of red numbers on the single zero (European) roulette
 *          wheel read counterclockwise.
 * https://oeis.org/A291156
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

/* Sequence of red numbers on the single zero (European) roulette wheel
   read counterclockwise.  The sequence consists of the 18 red numbers
   that appear in this order around the wheel; it repeats with period 18. */

seq_t a(idx_t n)
{
    /* Red numbers in counter‑clockwise order starting at the
       neighbour of 0 going counter‑clockwise (3 is the first). */
    static const seq_t RED[18] = {
        3, 12, 7, 18, 9, 14, 1, 16,
        5, 23, 30, 36, 27, 34, 25, 21,
        19, 32
    };
    /* Index into the period.  n ≥ 1. */
    idx_t p = (n - 1) % 18;
    return RED[p];
}
