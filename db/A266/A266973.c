/*
 * A266973  a(n) = 4^n mod 17.
 * https://oeis.org/A266973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n mod 17
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 4: [1, 4, 16, 13] */
seq_t a(idx_t n)
{
    static const seq_t table[4] = { (seq_t)1, (seq_t)4, (seq_t)16, (seq_t)13 };
    return table[n & 3ULL];
}
