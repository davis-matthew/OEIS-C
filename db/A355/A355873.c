/*
 * A355873  a(n) is the smallest positive exponent k such that the decimal
 *          expansion of n^k has at least one digit that occurs more than
 *          once.
 * https://oeis.org/A355873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the least k ≥ 1 such that n^k (in base 10) contains a
 *            digit appearing more than once
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9937 (2026-11-01)
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Any integer larger than 9876543210 has more than 10 digits,
       so its decimal expansion must contain a repeated digit. This
       handles the OEIS shortcut for large n. */
    if (n > 9876543210ULL)
        return (seq_t)1;

    seq_t base = (seq_t)n;
    seq_t power = base;            /* n^1 */
    unsigned int freq[10];

    /* Check k = 1 */
    digit_frequencies(power, freq);
    for (int d = 0; d < 10; ++d)
        if (freq[d] > 1)
            return (seq_t)1;

    /* Iterate k = 2,3,... until a repeated digit is found */
    for (idx_t k = 2; ; ++k)
    {
        power = mul_small(power, (unsigned int)n);   /* power *= n */
        digit_frequencies(power, freq);
        for (int d = 0; d < 10; ++d)
            if (freq[d] > 1)
                return (seq_t)k;
    }
}
