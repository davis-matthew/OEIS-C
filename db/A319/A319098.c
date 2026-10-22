/*
 * A319098  One of the three successive approximations up to 7^n for 7-adic
 *          integer 6^(1/3). This is the 5 (mod 7) case (except for n = 0).
 * https://oeis.org/A319098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique number k in [1,7^n] ≡ 5 (mod 7) with k³ ≡ 6
 *            (mod 7^n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: modular multiplication without overflow for 64‑bit seq_t */
static inline seq_t mod_mul_seq(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)((__uint128_t)a * b % mod);
#else
    return (seq_t)mod_mul((uint64_t)a, (uint64_t)b, (uint64_t)mod);
#endif
}

/* memoized sequence storage */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_last = 0;
static seq_t cur_k = 0;      /* last computed term a(cache_last) */
static seq_t cur_M = 0;      /* 7^cache_last */

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = (size_t)(n >= 1 ? n + 1 : 2);
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        for (size_t i = 0; i < cache_capacity; ++i) cache[i] = 0;
        cache[0] = 0;
        cache[1] = 5;          /* a(1) */
        cur_k = 5;
        cur_M = 7;             /* 7^1 */
        cache_last = 1;
    }

    /* compute missing terms up to n */
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t Mnext = cur_M * 7;          /* 7^(i) */

            /* k³ mod 7^(i) using safe multiplication */
            seq_t tmp = mod_mul_seq(cur_k, cur_k, Mnext);
            seq_t tpow = mod_mul_seq(tmp, cur_k, Mnext);

            seq_t numerator = tpow - (seq_t)6;
            seq_t div = numerator / cur_M;
            seq_t c = div % (seq_t)7;

            unsigned int c_int = (unsigned int)c;
            unsigned int t_int = (7 - (c_int * 3U) % 7) % 7;  /* t ≡ -c·3⁻¹ mod 7 */

            cur_k = cur_k + (seq_t)t_int * cur_M;  /* new k = k + t·7^i‑1 */
            cur_M = Mnext;                          /* new modulus */

            /* grow cache if necessary */
            if ((size_t)i >= cache_capacity) {
                cache_capacity = (size_t)i + 1;
                cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[i] = cur_k;
        }
        cache_last = n;
    }
    return cache[(size_t)n];
}
