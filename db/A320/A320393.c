/*
 * A320393  First members of the Cunningham chains of the first kind whose
 *          length is a prime.
 * https://oeis.org/A320393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that the Cunningham chain of * the
 *            first kind starting at p has a prime length
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

/* cache of found terms */
static seq_t *cache      = NULL;  /* array of seq_t values          */
static size_t cache_cap  = 0;     /* capacity of 'cache'            */
static size_t cache_len  = 0;     /* number of stored values        */
static seq_t  next_prime = 0;     /* next integer to test for prime*/

/* enlarge the cache if needed */
static void ensure_cache_cap(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function: returns the n‑th prime p such that the
 * Cunningham chain of the first kind starting at p has a prime length. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* not defined for n<1 */

    if (next_prime == 0) next_prime = 2;   /* lazy initialization */

    while (cache_len < (size_t)n) {
        /* find the next prime */
        while (!is_prime_big(next_prime))
            next_prime += 1;

        seq_t cand = next_prime;   /* candidate prime */
        idx_t  len = 1;            /* current chain length (including cand) */
        seq_t cur = cand;

        /* build the Cunningham chain of the first kind */
        while (true) {
            seq_t nxt = cur * 2 + 1;
            if (is_prime_big(nxt)) {
                ++len;
                cur = nxt;
            } else
                break;
        }

        /* if the length itself is prime, record the starting prime */
        if (is_prime_seq((uint64_t)len)) {
            ensure_cache_cap();
            cache[cache_len++] = cand;
        }

        next_prime += 1; /* continue searching after this candidate */
    }

    return cache[(size_t)n - 1];
}
