/*
 * A343280  a(n) is the number of steps for the n-th vertex of the Collatz
 *          tree A088975 to reach a vertex t == 0 (mod 3).
 * https://oeis.org/A343280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is periodic with period 9: 7, 6, 0, 5, 2, 0, 3, 4, 0. */
seq_t a(idx_t n)
{
    /* The sequence is 7,6,0,5,2,0,3,4,0 repeating from n = 1 onwards. */
    static const seq_t period[9] = { 7, 6, 0, 5, 2, 0, 3, 4, 0 };
    /* n is 1‑based; we assume n >= 1 (behavior for n == 0 is undefined). */
    idx_t i = (n - 1) % 9;
    return period[i];
}
