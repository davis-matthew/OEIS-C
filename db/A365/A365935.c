/*
 * A365935  Final digit (in decimal system) of n^(n+1) = A007778(n).
 * https://oeis.org/A365935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^(n+1) mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 gives 0^1 = 0 */
    if (n == 0) return (seq_t)0;

    /* Last digit of the base */
    unsigned int base = (unsigned int)(n % 10);

    /* Cycle lengths for powers modulo 10 */
    static const unsigned int cycle_len[10] = {
        1, /* 0 */
        1, /* 1 */
        4, /* 2 */
        4, /* 3 */
        2, /* 4 */
        1, /* 5 */
        1, /* 6 */
        4, /* 7 */
        4, /* 8 */
        2  /* 9 */
    };

    unsigned int cycle = cycle_len[base];

    /* Compute (n+1) modulo cycle.  Avoid overflow on n+1. */
    unsigned int exp_mod = ((unsigned int)(n % cycle) + 1) % cycle;
    if (exp_mod == 0)
        exp_mod = cycle;

    /* Compute base^exp_mod mod 10 */
    unsigned int result = 1;
    for (unsigned int i = 0; i < exp_mod; ++i)
        result = (result * base) % 10;

    return (seq_t)result;
}
