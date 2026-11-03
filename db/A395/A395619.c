/*
 * A395619  For any n > 0 with prime factorization Prod_{i = 1..k} p_i^e_i
 *          where p_1 < ... < p_k and e_1, ..., e_k > 0, a(n) = Prod_{i =
 *          1..k} prime(i)^a(e_i) (where prime(i) denotes the i-th prime
 *          numbers).
 * https://oeis.org/A395619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..k} prime(i)^{a(e_i)} where n = ∏_{i=1..k}
 *            p_i^{e_i} and p_1 < p_2 < … < p_k
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9902 (2026-11-03)
 * Helpers:   factor_u64, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Memoisation cache */
static seq_t  *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to n */
static void
ensure_cache_size(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = n * 2;
    seq_t *tmp = mem_resize(seq_t, cache, (new_cap * sizeof *cache) / sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, safety */
    if (n == 1) return 1;            /* a(1) = 1 */

    ensure_cache_size(n);
    if (cache[n - 1] != 0) return cache[n - 1];

    /* factorise n */
    uint64_t primes_arr[16];
    uint64_t exps_arr[16];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps_arr[i];
        /* prime index is i+1 (because p_1 < p_2 < ... < p_k) */
        int  prime_val = get_prime((idx_t)(i + 1));   /* global i‑th prime */
        seq_t exp_val  = a((idx_t)e);                 /* recursion on exponent */
        seq_t term     = ipow((seq_t)prime_val, exp_val);
        prod *= term;
    }

    cache[n - 1] = prod;
    return prod;
}
