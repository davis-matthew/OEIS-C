/*
 * A355200  Numbers k that can be written as the sum of 3 divisors of k (not
 *          necessarily distinct).
 * https://oeis.org/A355200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+6) = a(n) + 12; with a(1)=3, a(2)=4, a(3)=6, a(4)=8, a(5)=9,
 *            a(6)=12. Equivalently a(n) is the nth positive integer divisible
 *            by 3 or 4
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for one period of 6 terms (numbers 1..12 that are
       divisible by 3 or 4): 3, 4, 6, 8, 9, 12. */
    static const seq_t base[6] = { 3, 4, 6, 8, 9, 12 };
    /* Determine which block of 6 terms n falls into.  Each block
       increases the value by 12. */
    idx_t block = (n - 1) / 6;          /* 0‑based block index */
    idx_t offset = block * 12;          /* shift for the block */
    idx_t idx    = (n - 1) % 6;         /* position inside the block */
    return base[idx] + (seq_t)offset;
}
