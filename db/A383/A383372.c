/*
 * A383372  Number of centrally symmetric Baxter permutations of length n.
 * https://oeis.org/A383372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of centrally symmetric Baxter permutations of
 *            length n (OEIS A383372). Computation via the P‑recurrences given
 *            in the OEIS entry: - For even n = 2m, m ≥ 3: (m+1)² (3m – 2)
 *            a(2m) = (21m³ – 14m² – 5m + 6) a(2m – 2) + 8(m – 2)(m – 1)(3m +
 *            1) a(2m – 4). - For odd n = 2m+1, m ≥ 3: (m+1)(...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache can hold indices up to n (inclusive). */
static void enlarge_cache(idx_t n)
{
    if (n + 1u <= cache_cap)
        return;
    size_t new_cap = n + 1u;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1); /* allocation failure – terminate safely */
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1)            /* defensive guard – not expected */
        return 0;
    enlarge_cache(n);

    for (idx_t i = 0; i <= n; ++i) {
        if (i <= 3) {               /* base values */
            switch ((int)i) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)2; break;
                case 3: cache[3] = (seq_t)2; break;
            }
            continue;
        }

        if ((i & 1u) == 0u) {       /* even index: n = 2m */
            idx_t m = i >> 1;
            seq_t mm  = (seq_t)m;
            seq_t m2  = mm * mm;
            seq_t m3  = m2 * mm;

            /* denominator: (m+1)² * (3m – 2) */
            seq_t den = (mm + 1) * (mm + 1) * ((seq_t)3 * mm - 2);

            /* numerator terms */
            seq_t coef1 = ((seq_t)21 * m3
                           - (seq_t)14 * m2
                           - (seq_t)5 * mm
                           + 6);
            seq_t num1 = coef1 * cache[i-2];

            seq_t coef2 = (seq_t)8 * (mm - 2) * (mm - 1)
                               * ((seq_t)3 * mm + 1);
            seq_t num2 = coef2 * cache[i-4];

            cache[i] = (num1 + num2) / den;
        } else {                     /* odd index: n = 2m+1 */
            idx_t m = (i - 1) >> 1;
            seq_t mm  = (seq_t)m;
            seq_t m2  = mm * mm;
            seq_t m3  = m2 * mm;

            /* denominator: (m+1)(m+2)(3m – 1) */
            seq_t den = (mm + 1) * (mm + 2) * ((seq_t)3 * mm - 1);

            /* numerator terms (note indices shift: a(2m–1)=a(i–2), a(2m–3)=a(i–4)) */
            seq_t coef1 = ((seq_t)21 * m3
                           + (seq_t)14 * m2
                           - (seq_t)5 * mm
                           - 6);
            seq_t num1 = coef1 * cache[i-2];

            seq_t coef2 = (seq_t)8 * (mm - 1) * (mm - 1)
                               * ((seq_t)3 * mm + 2);
            seq_t num2 = coef2 * cache[i-4];

            cache[i] = (num1 + num2) / den;
        }
    }

    return cache[n];
}
