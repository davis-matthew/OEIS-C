/*
 * A269266  a(n) = 2^n mod 31.
 * https://oeis.org/A269266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n mod 5)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed powers of 2 modulo 31 up to period 5 */
    static const seq_t tbl[5] = {1, 2, 4, 8, 16};
    idx_t idx = n % 5;
    return tbl[idx];
}
