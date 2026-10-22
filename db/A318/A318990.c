/*
 * A318990  Numbers of the form prime(x) * prime(y) where x divides y.
 * https://oeis.org/A318990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(x) * prime(y) where x divides y (offset 1)
 * Range:     u8 none, u16 none, u32 n <= 1981, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine builds all such products for prime indices 1..20000,
   sorts them, removes duplicates, and returns the n‑th element. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* -------------------------------------------------------------------- */
/* Helper comparator for qsort – suitable for both C and C++ compilers */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* -------------------------------------------------------------------- */
/* Cached sequence – lazily constructed on the first call to a()        */
static seq_t *cache      = NULL;   /* sorted unique values              */
static size_t cache_len  = 0;      /* number of terms stored             */

/* -------------------------------------------------------------------- */
static void build_cache(void)
{
    /* Work only once */
    if (cache_len > 0) return;

    /* ---------------------------------------------------------------- */
    /* Parameters: use enough primes to generate the first ~10 000 terms */
    const idx_t N = 20000;                    /* number of prime indices to use          */
    const uint64_t PRIME_LIMIT = 320000;      /* gives at least 20000 primes             */

    /* ---------------------------------------------------------------- */
    /* Generate the first N primes                                 */
    uint64_t *primes   = NULL;
    size_t   prime_cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
    if (prime_cnt < (size_t)N) {                     /* unlikely */
        free(primes);
        return;
    }

    /* ---------------------------------------------------------------- */
    /* Collect all products prime(x)*prime(y) with x | y, 1 ≤ x ≤ y ≤ N  */
    size_t prod_cap = 0;
    size_t prod_cnt = 0;
    seq_t *prod = NULL;

    for (idx_t x = 1; x <= N; ++x) {
        const uint64_t px = primes[x - 1];
        for (idx_t k = 1;  ; ++k) {
            const idx_t y = x * k;
            if (y > N) break;
            const uint64_t py = primes[y - 1];
            const seq_t   val = (seq_t)px * (seq_t)py;

            /* Grow temporary array if needed */
            if (prod_cnt >= prod_cap) {
                size_t new_cap = prod_cap ? prod_cap << 1 : 1024;
                seq_t *tmp = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
                if (!tmp) {                               /* allocation failed */
                    free(prod);
                    free(primes);
                    return;
                }
                prod = tmp;
                prod_cap = new_cap;
            }
            prod[prod_cnt++] = val;
        }
    }

    /* ---------------------------------------------------------------- */
    /* Sort the array                                                 */
    qsort(prod, prod_cnt, sizeof(seq_t), cmp_seq);

    /* ---------------------------------------------------------------- */
    /* Deduplicate and store into the cache                           */
    if (!cache) {
        cache = (seq_t *)malloc(prod_cnt * sizeof(seq_t));
        if (!cache) {                               /* allocation failed */
            free(prod);
            free(primes);
            return;
        }
    }

    size_t unique = 0;
    for (size_t i = 0; i < prod_cnt; ++i) {
        if (i == 0 || prod[i] != prod[i - 1]) {
            cache[unique++] = prod[i];
        }
    }
    cache_len = unique;

    /* ---------------------------------------------------------------- */
    /* Clean up temporary data                                        */
    free(prod);
    free(primes);
}

/* -------------------------------------------------------------------- */
/* Public function: returns the n‑th term (n ≥ 1).                      */
seq_t a(idx_t n)
{
    if (cache_len == 0) build_cache();          /* lazy init */
    return (n <= cache_len) ? cache[n - 1] : (seq_t)0;
}
