/*
 * A257319  Numbers n such that the n-th generation of Sawtooth 201 has
 *          minimum population in Conway's Game of Life.
 * https://oeis.org/A257319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (47^(n-1) - 1) * 40
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0; a(n) = 47*a(n-1) + 1840 (n >= 2) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    seq_t res = 0;
    for (idx_t i = 2; i <= n; ++i) {
        res = res * (seq_t)47 + (seq_t)1840;
    }
    return res;
}
