/*
 * A290116  Primes of the form k! / 2 - 1.
 * https://oeis.org/A290116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form k!/2 – 1 * (k starts at 3,
 *            the first prime comes from 3! ). * The function generates
 *            factorials incrementally and * stores the primes it finds in a
 *            static cache
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *cache = NULL;      /* cache of primes found */
static size_t cache_len = 0;     /* number of cached primes */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* current factorial value; fact = last_k! */
static seq_t fact = 1;
static idx_t last_k = 1;         /* factorial of last_k (initially 1! = 1) */

seq_t a(idx_t n)
{
    /* return cached result if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary (using helper to handle arbitrary‑precision types) */
    if (cache_len >= cache_cap) {
        /* grow by doubling capacity or start at 16 */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* compute terms until we have at least n primes */
    while (cache_len < n) {
        /* advance to next k and update factorial */
        ++last_k;
        fact = mul_small(fact, (unsigned int)last_k);   /* fact = k! */

        /* candidate = (fact - 2) / 2 */
        seq_t cand = fact;
        cand = cand - (seq_t)2;
        cand = cand / (seq_t)2;

        /* check primality */
        if (is_prime_big(cand)) {
            /* ensure capacity for one more element */
            if (cache_len >= cache_cap) {
                grow_seq_array(&cache, &cache_cap, cache_cap * 2);
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
