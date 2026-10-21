/*
 * A274748  Number of nX7 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence for n > 15 a(n) = 21*a(n-1)
 *            -27*a(n-2) -407*a(n-3) +1614*a(n-4) -2305*a(n-5) +777*a(n-6)
 *            +1788*a(n-7) -2530*a(n-8) +1209*a(n-9) +56*a(n-10) -295*a(n-11)
 *            +115*a(n-12) -15*a(n-13); Initial values: a(1) = 32 a(2) = 448
 *            a(3) = 7858 a(4) = 143634 a(5) = 2661918 a(6) = 49...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is large enough and base values are initialized */
    if (seq_len == 0) {
        const char *base_strs[16] = {
            /* index 0 unused */ 0,
            "32", "448", "7858", "143634", "2661918", "49475642",
            "920432562", "17123659885", "318581114142", "5927090659144",
            "110271727008501", "2051571144042267", "38168848457457660",
            "710119627548581257", "13211556323824865000"
        };
        const int base_count = 15;
        if (n > (idx_t)base_count)
            seq_cap = (size_t)n + 1;
        else
            seq_cap = (size_t)base_count + 1;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq_cache) exit(1); /* allocation failure */
        for (idx_t i = 1; i <= base_count; ++i)
            seq_cache[i] = seq_from_str(base_strs[i]);
        seq_len = base_count;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        seq_cache = tmp;
        seq_cap = new_cap;
    }

    /* Compute new values up to n using the recurrence */
    if (n > seq_len) {
        static const int coeffs[13] = {
            21, -27, -407, 1614, -2305, 777,
            1788, -2530, 1209, 56, -295, 115, -15
        };
        for (idx_t k = seq_len + 1; k <= n; ++k) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int ci = 0; ci < 13; ++ci) {
                idx_t idx = k - (ci + 1);
                seq_t val = seq_cache[idx];
                int coeff = coeffs[ci];
                if (coeff > 0)
                    pos += (seq_t)coeff * val;
                else
                    neg += (seq_t)(-coeff) * val;
            }
            seq_cache[k] = pos - neg; /* pos ≥ neg for the sequence */
        }
        seq_len = n;
    }
    return seq_cache[n];
}
