/*
 * A299020  a(n) is the maximum digit in the factorial base expansion of 1/n.
 * https://oeis.org/A299020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum digit in factorial base expansion of 1/n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)          /* 1/1 has factorial‑base digit 1 */
        return (seq_t)1;

    uint64_t q = (uint64_t)n;      /* denominator (n) */
    uint64_t p = 1;                /* current numerator remainder */
    uint64_t max_d = 0;

    for (idx_t k = 2; ; ++k)
    {
        uint64_t prod = p * (uint64_t)k;   /* p * k fits in 64‑bit (≤ n²) */
        uint64_t d = prod / q;             /* digit at position k */
        if (d > max_d)
            max_d = d;
        p = prod % q;                      /* new remainder */
        if (p == 0)
            break;                         /* expansion terminates */
    }
    return (seq_t)max_d;
}
