/*
 * A341643  The unique strictly superior prime divisor of each number that has
 *          one.
 * https://oeis.org/A341643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the unique strictly superior prime divisor of the n‑th
 *            positive integer that has such a divisor
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (i.e. the prime factor p of n with p > sqrt(n), chosen from the sequence of numbers whose
 *           largest prime factor exceeds their square root). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/*--- global caches -------------------------------------------------------*/
static seq_t  *cache      = NULL;   /* store computed terms */
static size_t cache_size = 0;       /* number of terms already stored */
static size_t cache_cap  = 0;       /* allocated capacity of cache */

static idx_t last_processed = 1;    /* last integer examined for a prime divisor */

/*--- SPF (smallest prime factor) infrastructure --------------------------*/
static uint32_t *spf = NULL;        /* spf[i] holds the smallest prime factor of i */
static size_t    spf_limit = 0;      /* highest index safely indexed in spf */

static void build_spf(size_t limit)
{
    if (limit <= spf_limit) return;           /* already built enough */

    free(spf);
    spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);                        /* allocation failure */

    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {             /* avoid overflow on i*i */
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf[0] = spf[1] = 1;
    spf_limit = limit;
}

/*--- helper: largest prime factor of n ----------------------------------*/
static inline seq_t lpf_of(idx_t n)
{
    seq_t best = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        if (p > best) best = p;
        while (n % p == 0) n /= p;
    }
    return best;
}

/*--- public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* undefined, but guard */

    /* initial cache allocation */
    if (cache_cap == 0) {
        cache_cap   = 256;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_size  = 0;
    }

    /* expand cache if necessary (capacity only, not number of terms) */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap <<= 1;   /* double until enough */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms until we have n of them */
    while (cache_size < (size_t)n) {
        idx_t cur = last_processed + 1;            /* next integer to test */

        /* Ensure the SPF array covers 'cur' */
        if ((size_t)cur > spf_limit) {
            /* Pick a new limit at least double the current requirement */
            size_t new_limit = spf_limit ? spf_limit * 2 : 100000;
            if ((size_t)cur * 2 > new_limit) new_limit = (size_t)cur * 2;
            build_spf(new_limit);
        }

        seq_t lp = lpf_of(cur);                      /* largest prime factor */
        seq_t sq = floor_sqrt((seq_t)cur);           /* floor sqrt(cur) */
        if (lp > sq) {                               /* strictly superior prime found */
            cache[cache_size++] = lp;
        }

        last_processed = cur;
    }

    return cache[n - 1];
}
