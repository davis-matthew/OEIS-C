/*
 * A367511  Highly composite numbers h(k) = A002182(k) such that h >=
 *          rad(h)^2, where rad() = A007947().
 * https://oeis.org/A367511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th highly composite number h such that
   h ≥ rad(h)^2, where rad(h) is the product of distinct prime factors of h. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function to compute the n-th term of OEIS A367511. */
seq_t a(idx_t n)
{
    /* Static data for one‑time initialization. */
    static bool inited = false;
    static seq_t *cache = NULL;   /* array holding the sequence values */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    if (!inited) {
        /* ------------------------------------------------------------------
           Pre‑computation: generate all highly composite numbers up to a
           selected limit and keep those that satisfy the rad condition.
         ------------------------------------------------------------------ */
        const idx_t LIMIT = 2000000; /* sufficient to capture all known terms */

        /* SPF (smallest prime factor) sieve for divisor counts. */
        uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) return 0;        /* allocation failure – return zero */
        for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
        spf[0] = 0;
        spf[1] = 1;
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                if (i > LIMIT / i) continue;
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }

        /* Track divisor count thresholds (the "record" counts). */
        size_t max_divisors = 0;
        for (idx_t v = 1; v <= LIMIT; ++v) {
            /* Compute number of divisors d(v). */
            size_t d = 1;
            idx_t x = v;
            while (x > 1) {
                uint32_t p = spf[x];
                size_t exp = 0;
                while (x % p == 0) {
                    x /= p;
                    ++exp;
                }
                d *= (exp + 1);
            }

            /* Check for a new highly composite number. */
            if (d > max_divisors) {
                max_divisors = d;

                /* Compute rad(v) as product of distinct prime factors. */
                seq_t rad_val = 1;
                x = v;
                while (x > 1) {
                    uint32_t p = spf[x];
                    rad_val *= (seq_t)p;    /* multiply by the prime once */
                    while (x % p == 0) x /= p;
                }

                /* Condition: v >= rad(v)^2. */
                seq_t rad_sq = rad_val * rad_val;
                seq_t v_seq = (seq_t)v;
                if (v_seq >= rad_sq) {
                    /* Allocate or grow cache and store the value. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? 2 * cache_cap : 8;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) { free(spf); return 0; }
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = v_seq;
                }
            }
        }

        /* Clean up SPF array. */
        free(spf);
        inited = true;
    }

    /* Return the requested term or zero if the index is out of bounds. */
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
