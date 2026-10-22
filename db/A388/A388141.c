/*
 * A388141  Start the sequence S with a(1) = 1008973 and extend S with a(n)/2
 *          when a(n) is even, otherwise with a(n) + the smallest prime not
 *          yet added.
 * https://oeis.org/A388141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1008973; a(n+1) = a(n)/2 if a(n) even, else a(n)+p_k,
 *            where p_k is the k‑th smallest prime not yet used
 * Range:     u8 none, u16 none, u32 n <= 9182, u64 n <= 9182, u128 n <= 9182,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *cache      = NULL;   /* cache[1] holds a(1) */
static size_t cache_len  = 0;      /* highest index stored (0 if none) */
static size_t cache_cap  = 0;

static uint32_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static size_t    next_prime   = 0;  /* next unused prime index */

static void init_primes(void)
{
    if (prime_list != NULL) return;

    const size_t limit = 2000000;   /* enough for n up to 10 000 */
    uint64_t *u64p = NULL;
    size_t cnt = 0;

    build_primes_upto(limit, &u64p, &cnt);
    prime_cnt = cnt;
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_list[i] = (uint32_t)u64p[i];
    free(u64p);
}

static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* store the first term if not already done */
    if (cache_len < 1) {
        if (cache_cap < 2) {
            cache_cap = 16;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }
        cache[1] = (seq_t)1008973;
        cache_len = 1;
    }

    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t curr = cache[i];
        seq_t next;
        if ((curr & (seq_t)1) == 0) {          /* even */
            next = curr / (seq_t)2;
        } else {                               /* odd */
            next = curr + (seq_t)prime_list[next_prime++];
        }

        if (i + 1 >= cache_cap) {              /* grow storage if needed */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[i + 1] = next;
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but harmless */

    init_primes();
    ensure_cache(n);
    return cache[(size_t)n];
}
