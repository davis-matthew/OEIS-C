/*
 * A374457  The Dedekind psi function values of the exponentially odd numbers
 *          (A268335).
 * https://oeis.org/A374457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001615(A268335(n)) (Dedekind psi of the nth
 *            exponentially odd number)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* local power function for seq_t */
static inline seq_t pow_seq_local(seq_t base, int exp) {
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}

/* static cache for computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_m = 0;   /* last integer examined */

seq_t a(idx_t n) {
    /* Ensure cache is allocated */
    if (!cache) {
        cache_cap = 128;
        cache_len = 0;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms cached */
    while (cache_len < n) {
        uint64_t m = ++last_m;          /* next candidate integer */
        seq_t prod = 1;
        bool ok = true;
        uint64_t tmp = m;

        /* Handle factor 2 separately */
        if (tmp % 2 == 0) {
            int exp = 0;
            while (tmp % 2 == 0) {
                ++exp;
                tmp /= 2;
            }
            if (exp % 2 == 0) {
                ok = false;
            } else {
                prod *= pow_seq_local((seq_t)2, exp - 1);
                prod *= (seq_t)(2 + 1);
            }
        }

        /* Factor odd primes */
        for (uint64_t p = 3; p * p <= tmp; p += 2) {
            if (tmp % p != 0) continue;
            int exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            if (exp % 2 == 0) {
                ok = false;
                break;
            }
            prod *= pow_seq_local((seq_t)p, exp - 1);
            prod *= (seq_t)(p + 1);
        }

        if (!ok) continue;               /* not an exponentially odd number */

        /* Remaining prime factor, if any */
        if (tmp > 1) {
            prod *= (seq_t)(tmp + 1);     /* exponent is 1 (odd) */
        }

        /* Store the computed Dedekind psi */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp_ptr) exit(1);       /* allocation failed */
            cache = tmp_ptr;
            cache_cap = new_cap;
        }
        cache[cache_len++] = prod;
    }

    return cache[n - 1];
}
