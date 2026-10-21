/*
 * A289122  a(n) is number of odd integers divisible by 11 in the interval
 *          ]2*(n-1)^2, 2*n^2[.
 * https://oeis.org/A289122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n + 11*k) = a(n) + 2*k for n in [0,10] and k ≥ 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base values for n = 0 … 10: 0,0,0,1,0,1,1,1,2,1,2 */

seq_t a(idx_t n)
{
    static const seq_t base[11] = {
        0, 0, 0, 1, 0, 1, 1, 1, 2, 1, 2
    };
    idx_t k = n / 11;
    idx_t r = n % 11;
    return base[r] + (seq_t)2 * (seq_t)k;
}
