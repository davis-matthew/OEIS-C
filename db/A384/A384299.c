/*
 * A384299  Primes p such that p + 8, p + 12 and p + 20 are also primes.
 * https://oeis.org/A384299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that p+8, p+12, p+20 are all prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* Offset 1 */

    static seq_t *cache   = NULL;             /* Cached qualified primes */
    static size_t cache_size = 0;             /* Number of cached terms   */
    static size_t cache_cap  = 0;             /* Capacity of the cache    */
    static uint64_t last_prime = 0;           /* Last prime examined.     */

    const size_t need = (size_t)n;

    /* Expand the cache until we have at least `need` terms */
    while (cache_size < need) {
        /* Find the next prime after `last_prime` */
        uint64_t cand = last_prime + 1;
        if (cand < 2) cand = 2;
        else if ((cand & 1ULL) == 0) cand++;   /* Ensure odd candidate */

        while (!is_prime_big((seq_t)cand)) {
            if ((cand & 1ULL) == 0) cand += 1;
            else cand += 2;
        }
        last_prime = cand;                     /* cand is now prime        */

        /* Test the required pattern */
        seq_t p   = (seq_t)cand;
        seq_t p8  = p + (seq_t)8;
        seq_t p12 = p + (seq_t)12;
        seq_t p20 = p + (seq_t)20;

        if (is_prime_big(p8) && is_prime_big(p12) && is_prime_big(p20)) {
            /* Ensure capacity for the new term */
            if (cache == NULL) {
                cache_cap = 4;
                cache = seq_alloc(cache_cap);
            } else if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) {                /* allocation failed */
                    mem_free(cache);
                    cache = NULL;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = p;
        }
    }

    return cache[need - 1];
}
