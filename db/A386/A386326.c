/*
 * A386326  Primes without {0, 8} as digits.
 * https://oeis.org/A386326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime that contains no decimal digits 0 or 8
 * Range:     u8 n <= 47, u16 n <= 3471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9462 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes sequentially, testing the digit condition,
   and stores the values in a static cache so that calls with arbitrary
   indices are answered correctly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
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
    if (n == 0) return 0;           /* undefined for n < 1 – harmless */

    /* Static cache of found primes */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static uint64_t cand   = 2;      /* next integer to test */

    /* Grow cache until we have at least n values */
    while (len < n) {
        /* Test primality */
        if (is_prime_big((seq_t)cand)) {
            /* Check that none of its decimal digits are 0 or 8 */
            uint8_t buf[32];  /* enough for 128‑bit numbers */
            size_t dlen;
            get_digits((seq_t)cand, buf, &dlen);

            int ok = 1;
            for (size_t i = 0; i < dlen; ++i) {
                if (buf[i] == 0 || buf[i] == 8) {
                    ok = 0;
                    break;
                }
            }

            if (ok) {
                /* Allocate or grow the cache array */
                if (cache == NULL) {
                    cap = 8;
                    cache = seq_alloc(cap);
                } else if (len >= cap) {
                    size_t new_cap = cap << 1;
                    seq_t *tmp = seq_realloc(cache, cap, new_cap);
                    if (tmp == NULL) {   /* allocation failed */
                        exit(1);
                    }
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)cand;   /* store the prime */
            }
        }
        ++cand;  /* next integer to test */
    }

    return cache[n - 1];
}
