/*
 * A252890  Number of times the greatest prime factor of n^2 + 1 is a factor
 *          in all numbers <= n.
 * https://oeis.org/A252890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts, for n ≥ 1, the total multiplicity with which the
 *            greatest prime factor g of n²+1 appears as a factor in all
 *            numbers k²+1 for 1 ≤ k ≤ n. In other words, a(n) = Σ_{k=1..n}
 *            v_g(k²+1), where v_g(m) is the exponent of g in m
 * Range:     u8 n <= 9465, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1, but safe to return 0   */
        return (seq_t)0;

    /* greatest prime factor of n²+1 (fits in 64‑bit for n ≤ 10⁴) */
    uint64_t g = gpf((uint64_t)n * (uint64_t)n + 1ull);

    uint64_t count = 0;
    for (uint64_t k = 1; k <= n; ++k)
    {
        uint64_t v = k * k + 1ull;          /* k²+1 */
        while (v % g == 0)
        {
            ++count;
            v /= g;
        }
    }
    return (seq_t)count;
}
