/*
 * A379333  Numbers k such that lambda(k)^k == lambda(k) (mod k), where lambda
 *          = A002322.
 * https://oeis.org/A379333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A379333
    Numbers k such that λ(k)^k ≡ λ(k) (mod k),
    where λ is the Carmichael function A002322.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/*  Calculate Carmichael function λ(n) for a 64‑bit integer n.  */
static seq_t calc_lambda(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = n;

    /* Handle factor 2 separately. */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t lam;
        if (e == 1)          lam = (seq_t)1;
        else if (e == 2)     lam = (seq_t)2;
        else                lam = (seq_t)1 << (e - 2);   /* 2^(e-2) */

        seq_t g = gcd(result, lam);
        result = (result / g) * lam;
    }

    /* Factor odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t lam = (seq_t)(p - 1);
            for (uint64_t i = 1; i < e; ++i)
                lam *= (seq_t)p;          /* p^(e-1) */

            seq_t g = gcd(result, lam);
            result = (result / g) * lam;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        seq_t lam = (seq_t)(m - 1);      /* only power 1 */
        seq_t g = gcd(result, lam);
        result = (result / g) * lam;
    }

    return result;
}

/*  Test whether k satisfies λ(k)^k ≡ λ(k) (mod k).  */
static bool satisfies(uint64_t k)
{
    seq_t lam = calc_lambda(k);
    seq_t modk = (seq_t)k;
    seq_t pow_mod = mod_pow(lam, k, modk);
    return pow_mod == (lam % modk);
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    /* Static cache of sequence values. */
    static seq_t *cache     = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static uint64_t next_k   = 1;   /* next integer to test */

    /* Expand cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (satisfies(next_k)) {
            /* Grow storage if necessary. */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* In a production build, handle allocation failure.  */
                    abort();
                }
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
