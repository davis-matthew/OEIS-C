/*
 * A263841  Expansion of (1 - 2*x - x^2)/(sqrt(1+x)*(1-3*x)^(3/2)*2*x) -
 *          1/(2*x).
 * https://oeis.org/A263841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   -(n+1)*(n^2+n-3)*a(n) + 2*(n^3+3*n^2-4*n-3)*a(n-1) +
 *            3*(n-1)*(n^2+3*n-1)*a(n-2) = 0, a(0)=1, a(1)=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

#ifndef __cplusplus
/* C implementation – use 128‑bit arithmetic when available */
static inline seq_t safe_mul(seq_t a, seq_t b) {
#if defined(__SIZEOF_INT128__)
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#else
    return a * b;
#endif
}
#else
/* C++ implementation – use 128‑bit arithmetic only for integral types */
#include <type_traits>
static inline seq_t safe_mul(seq_t a, seq_t b) {
    if constexpr (std::is_integral<seq_t>::value) {
#if defined(__SIZEOF_INT128__)
        return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#else
        return a * b;
#endif
    } else {
        return a * b;
    }
}
#endif
static inline seq_t safe_add(seq_t a, seq_t b) { return a + b; }
static inline seq_t safe_sub(seq_t a, seq_t b) { return a - b; }

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = cache_cap; i <= idx; ++i) {
            if (i == 0) { cache[0] = (seq_t)1; continue; }
            if (i == 1) { cache[1] = (seq_t)3; continue; }

            seq_t n_seq     = (seq_t)i;
            seq_t n_minus1  = (seq_t)(i - 1);
            seq_t n_plus1   = (seq_t)(i + 1);

            seq_t n2 = safe_mul(n_seq, n_seq);
            seq_t n3 = safe_mul(n2, n_seq);

            /* f1 = n^3 + 3*n^2 - 4*n - 3 */
            seq_t f1 = safe_add(n3, safe_mul((seq_t)3, n2));
            f1 = safe_sub(f1, safe_mul((seq_t)4, n_seq));
            f1 = safe_sub(f1, (seq_t)3);

            /* f2 = n^2 + 3*n - 1 */
            seq_t f2 = safe_add(n2, safe_mul((seq_t)3, n_seq));
            f2 = safe_sub(f2, (seq_t)1);

            seq_t a_prev  = cache[i - 1];
            seq_t a_prev2 = cache[i - 2];

            seq_t term1 = safe_mul((seq_t)2, safe_mul(f1, a_prev));
            seq_t term2 = safe_mul((seq_t)3,
                                   safe_mul(n_minus1,
                                            safe_mul(f2, a_prev2)));
            seq_t numerator = safe_add(term1, term2);

            seq_t denom = safe_mul(n_plus1, safe_add(n2, safe_sub(n_seq, (seq_t)3)));

            cache[i] = numerator / denom;
        }
        cache_cap = new_cap;
    }
    return cache[idx];
}
