/*
 * A366424  The Bo Diddley beat. Periodic musical rhythm, embedded into a
 *          sequence of measured regular units of duration for both notes and
 *          rests. All terms except a(3 + 6*k) = 2 mark the lengths of notes.
 *          a(3 + 6*k) = 2 being a single rest of length 2 within each period
 *          of 6, where k >= 0.
 * https://oeis.org/A366424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = pattern[n % 6] where pattern = [3, 3, 2, 2, 2, 4]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {3, 3, 2, 2, 2, 4};
    return pattern[n % 6];
}
