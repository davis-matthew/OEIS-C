/*
 * A331840  Numbers k such that 30*k-13, 30*k-11 are twin primes.
 * https://oeis.org/A331840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 30*k-13 and 30*k-11 are twin
 *            primes
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of discovered terms and extends it only as needed.
   The next integer k is tested until enough terms are found.  Prime testing is performed with
   the provided is_prime_big() routine.  The cache grows geometrically to keep amortized
   time per call very small.  The algorithm is efficient enough for n up to about 10 000
   (k stays below 1 million). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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
    /* static cache of terms */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;   /* number of terms stored */
    static size_t   cache_cap = 0;   /* capacity of the cache array */
    static idx_t    next_k   = 1;    /* next k to test */

    if (n < 1) return 0;   /* undefined for n=0, but safety */

    /* if we already have the term, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache until we have n terms */
    while (cache_len < n) {
        idx_t k = next_k;
        ++next_k;

        /* compute the two candidate primes */
        seq_t p1 = (seq_t)(30 * k - 13);
        seq_t p2 = (seq_t)(30 * k - 11);

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* need more room in cache? */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
