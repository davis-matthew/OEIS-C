/*
 * A272590  a(n) is the smallest number m such that the multiplicative group
 *          modulo m is the direct product of n cyclic groups.
 * https://oeis.org/A272590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n==1?2:4*∏_{k=1}^{n-1}prime(k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/grow_seq.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* cache of a(n), 0‑based index */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* list of prime numbers, 1‑based (prime_list[0]==2) */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* product of the first (cache_len‑1) primes   (for i>=2) */
static seq_t product_primes = (seq_t)1;

/* ensure prime_list contains at least 'need' primes (need>=0) */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;
    /* upper bound for the nth prime (n = need) */
    idx_t limit = _prime_limit_for_idx((idx_t)need);
    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);
    free(prime_list);
    prime_list = tmp;
    prime_cnt = cnt;
}

/* ensure cache has room for 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) grow_seq(&cache, &cache_cap, need);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of bounds */

    /* pre‑allocate cache if required */
    ensure_cache((size_t)n);

    /* already computed? */
    if (cache_len >= (size_t)n) return cache[(size_t)n-1];

    /* prime list needs to be large enough for n‑1 primes */
    ensure_primes((size_t)n-1);

    /* compute missing values */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;     /* a(1) */
        } else {
            /* multiply by the (i‑2)‑th prime (0‑based) */
            size_t p_idx = i-2;
            product_primes = seq_mul(product_primes, (seq_t)prime_list[p_idx]);
            cache[i-1] = seq_mul(product_primes, (seq_t)4);
        }
    }
    cache_len = (size_t)n;
    return cache[(size_t)n-1];
}
