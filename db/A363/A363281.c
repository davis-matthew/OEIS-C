/*
 * A363281  Numbers which are the sum of 4 squares of distinct primes.
 * https://oeis.org/A363281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   The sequence is not given by a closed formula. It consists of
 *            all numbers that can be expressed as the sum of the squares of 4
 *            distinct prime numbers. The function below builds the list once
 *            (up to a sufficient prime limit), caches the sorted unique sums,
 *            and returns the n‑th element (1‑based)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9255 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

static seq_t *sum_cache = NULL;   /* array of unique sums */
static size_t cache_len  = 0;     /* number of sums stored */
static bool    cache_ready = false;

/* comparison for qsort */
static int seq_cmp(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* build the cache of all sums of squares of 4 distinct primes */
static void build_cache(void)
{
    /* Choose a prime limit that guarantees enough terms for n up to at least
       10,000.  The first 78 primes are ≤ 400.  78 choose 4 = 738,000 sums
       which is ample. */
    const idx_t PRIME_LIMIT = 400;

    size_t prime_cnt;
    uint64_t *primes;

    /* Generate all primes up to PRIME_LIMIT */
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);

    /* Estimate number of 4‑prime combinations */
    size_t total_combos = (size_t)prime_cnt * (prime_cnt - 1) *
                          (prime_cnt - 2) * (prime_cnt - 3) / 24;

    /* Allocate space for all sums */
    sum_cache = mem_new(seq_t, total_combos);
    if (!sum_cache) {                /* allocation failed – leave cache empty */
        free(primes);
        return;
    }

    /* Pre‑compute squares of each prime */
    seq_t *psq = mem_new(seq_t, prime_cnt);
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        psq[i] = p * p;
    }

    /* Enumerate all distinct 4‑prime combinations and store the sums */
    size_t idx = 0;
    for (size_t a = 0; a < prime_cnt - 3; ++a) {
        seq_t a2 = psq[a];
        for (size_t b = a + 1; b < prime_cnt - 2; ++b) {
            seq_t ab2 = a2 + psq[b];
            for (size_t c = b + 1; c < prime_cnt - 1; ++c) {
                seq_t abc2 = ab2 + psq[c];
                for (size_t d = c + 1; d < prime_cnt; ++d) {
                    sum_cache[idx++] = abc2 + psq[d];
                }
            }
        }
    }

    mem_free(psq);
    free(primes);

    if (idx == 0) {             /* nothing generated – should not happen */
        mem_free(sum_cache);
        sum_cache = NULL;
        cache_len = 0;
        cache_ready = true;
        return;
    }

    /* Sort all sums */
    mem_qsort(sum_cache, idx, sizeof(seq_t), seq_cmp);

    /* Deduplicate in place – keep first unique value at position 0 */
    size_t uniq = 0;
    for (size_t i = 0; i < idx; ++i) {
        if (uniq == 0 || seq_cmp(&sum_cache[i], &sum_cache[uniq - 1]) != 0) {
            sum_cache[uniq++] = sum_cache[i];
        }
    }
    cache_len = uniq;

    /* Resize buffer to the exact needed size */
    seq_t *tmp = mem_resize(seq_t, sum_cache, cache_len);
    if (tmp)
        sum_cache = tmp;

    cache_ready = true;
}

seq_t a(idx_t n)
{
    if (!cache_ready)
        build_cache();

    /* Indexing is 1‑based; return 0 for out‑of‑range requests */
    if (n == 0 || (size_t)(n - 1) >= cache_len)
        return (seq_t)0;

    return sum_cache[(size_t)(n - 1)];
}
