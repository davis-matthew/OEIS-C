/*
 * A367225  Numbers m without a divisor whose prime indices sum to
 *          bigomega(m).
 * https://oeis.org/A367225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367225
   Numbers m without a divisor whose prime indices sum to bigomega(m) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* utility from the build system */
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /*----- static cache for already found terms -----*/
    static seq_t *cache = NULL;    /* array of seq_t values */
    static size_t cache_cap = 0;   /* current allocated capacity */
    static size_t cache_len = 0;   /* number of terms stored */

    /*----- static data for factorization -----*/
    static idx_t *spf = NULL;          /* smallest prime factor array        */
    static size_t spf_limit = 0;       /* allocated size of spf array        */
    static uint32_t *prime_index = NULL;/* prime → its index (1‑based)        */
    static size_t prime_cnt = 0;       /* number of primes seen in range     */

    static idx_t last_checked = 1;     /* largest m already examined        */

    /*--- grow cache if needed -----*/
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /*--- return the term if already known ---*/
    if (n <= cache_len) return cache[n - 1];

    /*--- initialise spf & prime_index tables once ---*/
    if (!spf) {
        const size_t INIT_SPF_LIMIT = 5000000;      /* enough for n≈10000 */
        spf_limit = INIT_SPF_LIMIT;
        build_spf(spf_limit, &spf);

        prime_index = (uint32_t *)calloc(spf_limit + 1, sizeof(uint32_t));
        for (idx_t i = 2; i <= spf_limit; ++i) {
            if (spf[i] == i) {  /* i is prime */
                prime_cnt++;
                prime_index[i] = (uint32_t)prime_cnt;   /* indices start at 1 */
            }
        }
    }

    /*--- main search loop: find more terms until we have n of them ---*/
    for (idx_t m = last_checked + 1;; ++m) {
        /* enlarge SPF table if necessary */
        if ((size_t)m > spf_limit) {
            size_t new_limit = spf_limit * 2;
            if (new_limit < (size_t)m * 2) new_limit = m * 2;   /* safety */
            free(spf);
            free(prime_index);

            spf_limit = new_limit;
            build_spf(spf_limit, &spf);
            prime_index = (uint32_t *)calloc(spf_limit + 1, sizeof(uint32_t));

            prime_cnt = 0;
            for (idx_t i = 2; i <= spf_limit; ++i) {
                if (spf[i] == i) {
                    prime_cnt++;
                    prime_index[i] = (uint32_t)prime_cnt;
                }
            }
        }

        /* factorise m, collect prime indices and multiplicities */
        idx_t x = m;
        int weights[32] = {0};
        int counts[32]  = {0};
        int num_primes  = 0;
        int bigomega    = 0;  /* total exponent count */

        while (x > 1) {
            idx_t p  = spf[x];
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }

            int idx = (int)prime_index[(uint32_t)p];   /* prime index */
            weights[num_primes] = idx;
            counts[num_primes]  = e;
            bigomega += e;
            ++num_primes;
        }

        /* check whether some divisor has prime-index sum equal to bigomega */
        bool reachable[128] = {false};
        reachable[0] = true;

        for (int i = 0; i < num_primes; ++i) {
            int w = weights[i];
            int e = counts[i];
            for (int step = 0; step < e; ++step) {
                for (int s = bigomega; s >= w; --s) {
                    if (reachable[s - w]) reachable[s] = true;
                }
            }
        }

        /* m qualifies if no divisor reaches the target sum */
        if (!reachable[bigomega] && bigomega > 0) {
            cache[cache_len] = (seq_t)m;
            ++cache_len;

            if (n <= cache_len) {
                last_checked = m;
                return cache[n - 1];
            }
        }

        last_checked = m;
    }
}
