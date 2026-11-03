/*
 * A284412  a(1) = 1, a(2) = 2; a(n) is the largest prime <= (a(a(n-1)) +
 *          a(n-a(n-1))) for n > 2.
 * https://oeis.org/A284412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; a(n) = largest prime ≤ (a(a(n‑1)) +
 *            a(n‑a(n‑1))) for n > 2
 * Range:     u8 n <= 503, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9213 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Ensure the cache can hold the requested index */
    size_t nn = (size_t)n;
    if (nn >= cache_size) {
        size_t new_size = cache_size > 0 ? cache_size : 16;
        while (new_size <= nn) new_size <<= 1;

        seq_t *tmp = (cache_size == 0) ?
                         seq_alloc(new_size) :
                         seq_realloc(cache, cache_size, new_size);

        if (!tmp) { /* allocation failure */
            /* In a real program you would handle this gracefully. */
            while (true) { } /* unreachable */
        }

        /* Zero newly allocated elements (they are zero‑valued). */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;

        /* Initialise the only defined terms if the cache was just created. */
        if (cache == NULL) {
            tmp[1] = 1;
            tmp[2] = 2;
        }
        cache      = tmp;
        cache_size = new_size;
    }

    if (cache[nn] != 0)        /* Already computed */
        return cache[nn];

    /* Recursive computation with memoisation */
    seq_t prev      = a(n - 1);                     /* a(n‑1)   */
    seq_t inner     = a((idx_t)prev);               /* a(a(n‑1)) */
    idx_t second_idx = n - (idx_t)prev;             /* n‑a(n‑1)  */
    seq_t inner2    = a(second_idx);                /* a(n‑a(n‑1)) */
    seq_t sum        = inner + inner2;

    /* Find the largest prime ≤ sum */
    seq_t candidate = sum;
    while (!is_prime_big(candidate)) {
        /* The sequence only takes positive values, so condition below is safe. */
        if (candidate == 0) break;
        --candidate;
    }

    cache[nn] = candidate;
    return candidate;
}
