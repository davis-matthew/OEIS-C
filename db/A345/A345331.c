/*
 * A345331  Odd numbers k > 1 such that m^(2^v(k-1)+1) == -m (mod k) has more
 *          than one solution modulo k, where v(k) = A007814(k) is the 2-adic
 *          valuation of k.
 * https://oeis.org/A345331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345331: odd composite k > 1 such that some prime factor p of k satisfies
   v2(p‑1) > v2(k‑1), where v2(x) is the 2‑adic valuation of x.
   The function generates terms by incremental sieving until the requested
   index n is reached. */
/* This implementation uses a dynamic cache; it works for 64‑bit
   values as well as for arbitrary‑precision seq_t. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* 2‑adic valuation for uint64_t. */
static inline unsigned v2_uint64(uint64_t x)
{
    /* x must be > 0 */
    return __builtin_ctzll(x);
}

/* Cache storage.  cache[i] holds the (i+1)‑th term (1‑based). */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static uint64_t checked_limit = 1;   /* last odd number examined */

/* Ensure the cache has at least "needed" capacity. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (newcap < needed)
        newcap = needed;
    /* realloc may return NULL on failure; cast to seq_t* for C++ */
    void *tmp = realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = (seq_t *)tmp;
    cache_cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is 1‑based; undefined for n==0 */
        abort();

    /* Extend cache until we have at least n terms. */
    if ((size_t)n > cache_size) {
        ensure_cache_capacity((size_t)n);

        uint64_t k = checked_limit + 1;
        if ((k & 1u) == 0)      /* make odd */
            k++;

        for ( /* no initialization */ ; cache_size < (size_t)n; k += 2) {
            /* Factor k. */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(k, primes, exps, &cnt);

            /* Skip if k is prime. */
            if (cnt == 1 && exps[0] == 1)
                continue;

            unsigned vk1 = v2_uint64(k - 1);
            bool ok = false;
            for (size_t i = 0; i < cnt && !ok; ++i) {
                unsigned vp1 = v2_uint64(primes[i] - 1);
                if (vp1 > vk1)
                    ok = true;
            }
            if (ok)
                cache[cache_size++] = (seq_t)k;
        }
        /* k has been incremented once after the last processed odd. */
        checked_limit = k - 2;
    }

    return cache[(size_t)n - 1];
}
