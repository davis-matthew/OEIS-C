/*
 * A348884  Number of pairs of base-p digits for which binomial coefficients
 *          support a Lucas congruence modulo p^2, where p is the n-th prime.
 * https://oeis.org/A348884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   count_pairs_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348884: Number of pairs of base‑p digits for which the
   Lucas congruence holds modulo p², where p is the n‑th prime. */
/* Computation is performed by directly checking the harmonic
   conditions for each prime.  A memoised cache of results is kept
   per index; primes are generated once up to a fixed upper bound
   that safely contains the 10 000th prime. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "../utils/count_pairs_for_prime.h"

/* ------------------------------------------------------------ */
/* Prime generation ------------------------------------------------- */
static void build_primes_upto_limit(idx_t limit, uint32_t **out, size_t *cnt)
{
    size_t n = (size_t)limit;
    unsigned char *is_comp = (unsigned char *)calloc(n + 1, 1);
    if (!is_comp) return;

    size_t sq = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sq; ++p) {
        if (!is_comp[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                is_comp[j] = 1;
        }
    }

    size_t primes_est = n / (log((double)limit) - 1); /* rough estimate */
    uint32_t *pr = (uint32_t *)malloc(primes_est * sizeof(uint32_t));
    size_t pc = 0;
    for (size_t x = 2; x <= limit; ++x) {
        if (!is_comp[x]) {
            if (pc == primes_est) {
                primes_est *= 2;
                pr = (uint32_t *)realloc(pr, primes_est * sizeof(uint32_t));
            }
            pr[pc++] = (uint32_t)x;
        }
    }
    free(is_comp);

    *out = pr;
    *cnt = pc;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* --- Static cache for results --- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t computed_up_to = 0;

    if (n < 1) return 0;   /* undefined for zero or negative indices */

    /* Ensure cache array is large enough */
    if (cache == NULL || cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache && cache == NULL)
            memset(new_cache, 0, new_cap * sizeof(seq_t));           /* init first time */
        if (new_cache && cache && cache_cap < new_cap)
            memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure primes array covers the required n-th prime */
    if (primes == NULL || primes_cnt < (size_t)n) {
        /* 2000000 safely contains the 10 000th prime (104729) */
        static const idx_t PRIME_LIMIT = 2000000;
        uint32_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto_limit(PRIME_LIMIT, &tmp, &cnt);
        primes = tmp;
        primes_cnt = cnt;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (cache[i] != 0) continue;     /* already computed */
        uint32_t p = primes[(size_t)i - 1];      /* primes are 0‑based */
        cache[i] = count_pairs_for_prime(p);
    }
    computed_up_to = n;

    return cache[n];
}
