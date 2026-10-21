/*
 * A270346  a(n) is the number whose base-11 digits are, in order, the first n
 *          terms of the simple periodic sequence: repeat 2,3,5,7.
 * https://oeis.org/A270346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=2; for n>1, a(n) = a(n-1)*11 + d(n)
   where d(n) cycles through 2,3,5,7 (the first primes) */
seq_t a(idx_t n)
{
    static const unsigned pattern[4] = { 2u, 3u, 5u, 7u };
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned d = pattern[(i - 1) & 3];
        value = value * (seq_t)11 + (seq_t)d;
    }
    return value;
}
