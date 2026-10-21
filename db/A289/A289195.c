/*
 * A289195  a(n) is the number of odd integers divisible by 5 in ]4*(n-1)^2,
 *          4*n^2[.
 * https://oeis.org/A289195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0..4)[n mod 5] + 4 * floor(n/5) * Recurrence: a(n + 5k)
 *            = a(n) + 4k * Base values for n mod 5 = 0..4: 0, 0, 2, 2, 2
 * Range:     u8 n <= 319, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base pattern for remainders 0..4 */
    static const seq_t base[5] = {0, 0, 2, 2, 2};

    idx_t rem = n % 5;          /* remainder modulo 5 */
    seq_t part = (seq_t)(n / 5) * 4;   /* 4 * floor(n/5) */

    return part + base[rem];
}
