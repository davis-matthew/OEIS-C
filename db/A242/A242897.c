/*
 * A242897  Catalan numbers C(n) such that sum of the factorials of digits of
 *          C(n) is semiprime.
 * https://oeis.org/A242897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   catalan, factor_u64, get_digits, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A242897: a(n) is the n-th Catalan number C(k) (k≥4) such that
 * the sum of the factorials of its decimal digits is a semiprime. */
/* The function finds successive Catalan numbers, tests the condition,
 * stores the qualifying ones and returns the requested term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/catalan.h"       /* catalan(idx_t k) */
#include "../utils/get_digits.h"    /* get_digits(seq_t x, uint8_t *buf, size_t *len) */
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static const uint64_t digit_fact[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL, 40320ULL, 362880ULL
};

seq_t a(idx_t n)
{
    /* undefined for n==0; offset is 1 */
    if (n == 0) return 0;

    /* Static state: cache of qualifying Catalan numbers */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored good terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    /* Next Catalan index to examine (start with 4 so C(4)=14) */
    static idx_t next_k = 4;

    /* Grow the cache until we have at least n qualifying terms */
    while (cache_len < n) {
        /* Compute next Catalan number C(next_k) */
        seq_t cat = catalan(next_k);
        ++next_k;

        /* Obtain its decimal digits */
        /* Buffer large enough for Catalan numbers of indices up to several ten‑thousands */
        enum { DIG_BUF_SZ = 200000 };
        uint8_t digit_buf[DIG_BUF_SZ];
        size_t digit_len;
        get_digits(cat, digit_buf, &digit_len);

        /* Sum factorials of digits */
        uint64_t sum = 0;
        for (size_t i = 0; i < digit_len; ++i) {
            sum += digit_fact[digit_buf[i]];
        }

        /* Test whether sum is a semiprime (p^2 or p*q, p≠q) */
        bool semiprime = false;
        if (sum > 1) {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t pcnt;
            factor_u64(sum, primes, exps, &pcnt);
            if ((pcnt == 1 && exps[0] == 2) ||
                (pcnt == 2 && exps[0] == 1 && exps[1] == 1))
                semiprime = true;
        }

        /* If it qualifies, store it */
        if (semiprime) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();      /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cat;
        }
    }

    return cache[n - 1];
}
