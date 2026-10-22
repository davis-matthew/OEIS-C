/*
 * A312508  Coordination sequence Gal.5.250.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -3*a(n-1) -6*a(n-2) -9*a(n-3) -11*a(n-4) -11*a(n-5)
 *            -8*a(n-6) -2*a(n-7) +6*a(n-8) +14*a(n-9)
 *            +20*a(n-10)+22*a(n-11)+20*a(n-12)+14*a(n-13)+6*a(n-14)
 *            -2*a(n-15)-8*a(n-16)-11*a(n-17)-11*a(n-18)-9*a(n-19)-6*a(n-20)-3*a(n-21)-a(n-22)
 *            for n > 23, with initial values a(0)..a(23) given in the OEIS...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 24 terms */
    static const seq_t init[24] = {
        1, 4, 8, 14, 18, 22, 30, 30, 36, 42,
        44, 56, 46, 64, 66, 64, 74, 82, 76, 90,
        90, 98, 96, 112
    };

    /* memoisation storage */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;
    static size_t memo_cap = 0;

    /* initialise memo with the first 24 terms */
    if (!memo) {
        memo_cap = 24;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        for (size_t i = 0; i < 24; ++i) memo[i] = init[i];
        memo_len = 24;
    }

    /* extend storage if necessary */
    if ((size_t)n >= memo_cap) {
        size_t new_cap = memo_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        memo = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;
    }

    /* if the requested term is already known, return it */
    if ((size_t)n < memo_len) return memo[(size_t)n];

    /* recurrence coefficients (22 terms) */
    const int coeffs[22] = {
        -3, -6, -9, -11, -11, -8, -2,
         6, 14, 20, 22, 20, 14, 6,
        -2, -8, -11, -11, -9, -6, -3, -1
    };

    /* compute missing terms up to n */
    for (idx_t k = (idx_t)memo_len; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int i = 0; i < 22; ++i) {
            seq_t val = memo[(size_t)(k - 1 - i)];
            int c = coeffs[i];
            if (c > 0)
                sum_pos += (seq_t)c * val;
            else
                sum_neg += (seq_t)(-c) * val;
        }
        memo[(size_t)k] = sum_pos - sum_neg;
    }
    memo_len = (size_t)(n + 1);
    return memo[(size_t)n];
}
