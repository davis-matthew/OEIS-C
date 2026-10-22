/*
 * A368832  Integers not of one of the 5 forms p^k, p*q^k, 2*p*q^k, p*q*r or
 *          2*p*q*r with p, q, r distinct primes and k>=0.
 * https://oeis.org/A368832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer not of the forms * p^k , p*q^k
 *            , 2*p*q^k , p*q*r or 2*p*q*r (p,q,r distinct primes, k≥0)
 * Range:     u8 n <= 22, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether x is NOT of any forbidden form. */
static bool allowed(uint64_t x) {
    if (x < 2) return false;           /* 1 is forbidden via p^0 */

    uint64_t n = x;
    int primes[10];
    int exps[10];
    int cnt = 0;

    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[cnt] = (int)d;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        primes[cnt] = (int)n;
        exps[cnt]   = 1;
        ++cnt;
    }

    if (cnt == 1)                    /* prime power p^k */
        return false;

    if (cnt == 2) {                   /* p*q^k, one exponent must be 1 */
        if (exps[0] == 1 || exps[1] == 1)
            return false;
        return true;
    }

    if (cnt == 3) {
        /* p*q*r (all exps 1) */
        if (exps[0] == 1 && exps[1] == 1 && exps[2] == 1)
            return false;

        /* 2*p*q^k with 2^1, one other prime exponent 1, the third >=2 */
        if (primes[0] == 2 && exps[0] == 1) {
            if ((exps[1] == 1 && exps[2] >= 2) ||
                (exps[1] >= 2 && exps[2] == 1))
                return false;
        }
        return true;
    }

    if (cnt == 4) {                    /* 2*p*q*r with all exps 1 */
        bool have2 = false, all1 = true;
        for (int i = 0; i < 4; ++i) {
            if (primes[i] == 2)
                have2 = true;
            if (exps[i] != 1)
                all1 = false;
        }
        if (have2 && all1)
            return false;
        return true;
    }

    /* cnt >= 5: never matches any forbidden form */
    return true;
}

/* Return a(n) for OEIS A368832.  n≥1 */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                       /* undefined, harmless */

    static seq_t *cache      = NULL;     /* memoised terms */
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t next_candidate = 2;   /* candidate to test next */

    /* grow cache to hold at least n terms */
    while (cache_len < n) {
        /* test next_candidate */
        if (allowed(next_candidate)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
