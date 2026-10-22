/*
 * A351151  G.f. A(x) satisfies: A(x) = 1 + x^2 * A(x/(1 - 5*x)) / (1 - 5*x).
 * https://oeis.org/A351151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0; for n ≥ 2, a(n) = Σ_{k=0}^{n-2}
 *            binomial(n-2,k) * 5^k * a(n-2-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t  cap    = 0;      /* cache capacity   */
    static size_t  computed = 0;    /* highest n computed (cache[computed] is defined) */

    /* static array of powers of 5: pow5[k] = 5^k */
    static seq_t *pow5  = NULL;
    static size_t pow_cap = 0;      /* size of pow5 array */

    /* Ensure cache has room for index n */
    size_t need_cache = (size_t)n + 1;
    if (need_cache > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need_cache * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* OOM fallback: 0 */
        cache = tmp;
        cap   = need_cache;
    }

    /* Ensure pow5 can provide 5^k up to k = n-2 (i.e. up to size n) */
    size_t need_pow = (size_t)n;             /* we need indices 0 .. n-1 */
    if (need_pow > pow_cap) {
        seq_t *tmp = (seq_t *)realloc(pow5, need_pow * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        pow5 = tmp;

        size_t start;
        if (pow_cap == 0) {                 /* first allocation */
            pow5[0] = (seq_t)1;
            start = 1;
        } else {
            start = pow_cap;
        }
        for (size_t i = start; i < need_pow; ++i)
            pow5[i] = pow5[i-1] * (seq_t)5;
        pow_cap = need_pow;
    }

    /* Initialize base values once */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        if (need_cache > 1)
            cache[1] = (seq_t)0;
        computed = 1;
    }

    /* Compute values up to n (independently of call order) */
    while (computed < (size_t)n) {
        size_t i = computed + 1;                  /* index to compute */
        seq_t sum = (seq_t)0;
        size_t len = i - 2;                       /* upper bound for k */

        seq_t C = (seq_t)1;                       /* binomial C(len,0) */
        for (size_t k = 0; k <= len; ++k) {
            seq_t Cp   = C * pow5[k];
            seq_t term = Cp * cache[i - 2 - k];
            sum += term;

            if (k < len) {
                seq_t num = (seq_t)(len - k);
                seq_t den = (seq_t)(k + 1);
                C = (C * num) / den;
            }
        }
        cache[i] = sum;
        computed = i;
    }

    return cache[(size_t)n];
}
