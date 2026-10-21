/*
 * A253106  Semiprimes with smallest factor <= 3.
 * https://oeis.org/A253106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_local_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth smallest integer of the form 2p or 3p,
 * where p is prime (semiprimes with smallest factor <= 3). 
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_local_cmp.h"

/* Static cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t prime_limit = 200000;   /* initial prime limit to generate */
static bool seq_initialized = false;

/* Generate the sequence up to at least 'need' terms.
   prime_limit will be increased automatically if needed. */
static void generate_sequence(size_t need)
{
    while (seq_len < need) {
        /* Get primes up to current limit */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(prime_limit, &primes, &prime_cnt);

        /* Allocate temporary array for candidate values */
        size_t tmp_cap = 2 * prime_cnt + 10;
        seq_t *tmp = (seq_t *)malloc(tmp_cap * sizeof(seq_t));
        size_t tmp_len = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            tmp[tmp_len++] = (seq_t)(2 * (seq_t)p);
            tmp[tmp_len++] = (seq_t)(3 * (seq_t)p);
        }
        free(primes);

        /* Sort the values */
        qsort(tmp, tmp_len, sizeof(seq_t), seq_local_cmp);

        /* Remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < tmp_len; ++i) {
            if (uniq == 0 || tmp[i] != tmp[uniq - 1]) {
                tmp[uniq++] = tmp[i];
            }
        }

        /* Replace the cached sequence */
        free(seq_cache);
        seq_cache = tmp;
        seq_len = uniq;

        /* If still not enough, enlarge the prime limit and repeat */
        if (seq_len < need) {
            prime_limit *= 2;   /* double the limit */
        }
    }
    seq_initialized = true;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is 1‑based */

    if (!seq_initialized) {
        generate_sequence((size_t)n);
    } else if (seq_len < (size_t)n) {
        generate_sequence((size_t)n);
    }

    return seq_cache[n - 1];
}
