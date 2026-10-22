/*
 * A335167  Nim n-th power of 8.
 * https://oeis.org/A335167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n+5) = a(n) with a(0)=1, a(1)=8, a(2)=13, a(3)=14, a(4)=10 */
seq_t a(idx_t n)
{
    /* The sequence repeats every 5 terms, so we compute n mod 5. */
    static const seq_t table[5] = {1, 8, 13, 14, 10};
    return table[n % 5];
}
