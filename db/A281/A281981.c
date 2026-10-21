/*
 * A281981  a(n) = 4*Sum_{i=1..n-1} Sum_{j=1..m} floor((j*i)/n) -
 *          (m-1)*m*(n-1) where m is floor(sqrt(n)).
 * https://oeis.org/A281981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Sum_{i=1..n-1} Sum_{j=1..m} floor((j*i)/n) -
 *            (m-1)*m*(n-1) where m = floor(sqrt(n)). Offset: 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for 1ULL constants */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t x = 0, bit = 1ULL << 62; /* largest power of 4 ≤ 2⁶⁴ */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety; offset starts at 1 */
    idx_t m = floor_sqrt_u64(n);
    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i) {
        seq_t inner = 0;
        for (idx_t j = 1; j <= m; ++j) {
            inner += (seq_t)((i * j) / n);
        }
        sum += inner;
    }
    seq_t term1 = sum * 4;
    seq_t term2 = (seq_t)(m - 1) * (seq_t)m * (seq_t)(n - 1);
    return term1 - term2;
}
