/*
 * A319871  a(n) = 7*6*5*4*3*2*1 + 14*13*12*11*10*9*8 + ... + (up to the n-th
 *          term).
 * https://oeis.org/A319871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..6} [ (n-j)%7==0 ] * Prod_{i=1..j} (n - i - j +
 *            8) * + Sum_{m=7,14,21,... <= n} Prod_{t=0..6} (m - t) (OEIS
 *            A319871)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 426,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety for offset 1 */

    const idx_t k = 7;         /* block size */
    unsigned __int128 total = 0;

    /* First sum: partial blocks */
    for (idx_t j = 1; j < k; ++j) {
        if (n >= j && ((n - j) % k) == 0) {
            unsigned __int128 prod = 1;
            for (idx_t i = 1; i <= j; ++i) {
                int64_t term = (int64_t)n - (int64_t)i - (int64_t)j
                               + (int64_t)k + 1;
                prod *= (unsigned __int128)(uint64_t)term;
            }
            total += prod;
        }
    }

    /* Second sum: full blocks (multiples of k) */
    for (idx_t m = k; m <= n; m += k) {
        unsigned __int128 prod = 1;
        for (idx_t t = 0; t < k; ++t) {
            prod *= (unsigned __int128)(m - t);
        }
        total += prod;
    }

    return (seq_t)total;
}
