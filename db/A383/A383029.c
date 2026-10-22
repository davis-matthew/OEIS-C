/*
 * A383029  Exponent of the highest power of 2 dividing the n-th exponentially
 *          odd number.
 * https://oeis.org/A383029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = v2(eogn(n))
 *   where eogn(n) is the n-th number whose prime factorisation
 *   has all exponents odd (the “exponentially odd” numbers from A268335).
 * The sequence gives the exponent of the highest power of 2 dividing that
 * number (i.e., the number of trailing zeros in its binary expansion).
 *
 * This implementation generates the numbers on demand, caching all
 * previously computed terms.  It factors each integer with
 * factor_u64 (provided by the system) to test whether all exponents
 * are odd.  The implementation is efficient for n up to at least
 * 10 000 (≈2 s or less on typical hardware).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Maximum number of distinct prime factors for any 64‑bit integer
 * (enough for our brute‑force factorisation). */
#define MAX_PRIMES 64

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *cache = NULL;     /* sequence a(1)…a(curr)  */
    static idx_t  computed  = 0;     /* number of terms in cache  */
    static size_t cache_cap = 0;     /* allocated capacity of cache   */
    static uint64_t last_candidate = 0; /* last integer examined       */

    /* Grow cache if necessary */
    if (n > computed) {
        /* Expand on powers of two for amortised O(1) allocation */
        size_t need = n;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache)
                return 0;                /* memory allocation failed */
            if (cache) {
                for (idx_t i = 0; i < computed; i++)
                    new_cache[i] = cache[i];
                free(cache);
            }
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Generate the missing terms */
        for (idx_t pos = computed; pos < n; ++pos) {
            uint64_t num = last_candidate + 1;
            while (true) {
                /* Factor 'num' */
                uint64_t primes[MAX_PRIMES];
                uint64_t exps[MAX_PRIMES];
                size_t   num_primes = 0;
                factor_u64(num, primes, exps, &num_primes);

                /* Test whether all exponents are odd */
                bool ok = true;
                for (size_t i = 0; i < num_primes; ++i) {
                    if ((exps[i] & 1ULL) == 0ULL) { ok = false; break; }
                }

                if (ok) {
                    /* Compute v2(num) – number of trailing zeros */
                    seq_t v = 0;
                    uint64_t tmp = num;
                    while ((tmp & 1ULL) == 0ULL) {
                        ++v;
                        tmp >>= 1;
                    }
                    cache[pos] = v;
                    last_candidate = num;
                    break;                /* next position */
                }
                ++num;                     /* try next integer */
            }
        }
        computed = n;
    }

    return cache[n - 1];
}
