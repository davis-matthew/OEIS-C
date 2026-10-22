/*
 * A345298  a(n) = Sum_{p|n, p prime} tau(p #).
 * https://oeis.org/A345298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p‖n} 2^π(p) (sum over distinct prime divisors p of n)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for primes */
static uint64_t *primes = NULL;
static size_t    primes_cnt = 0;
static size_t    primes_cap = 0;
static uint64_t  primes_limit = 1;

/* static storage for powers of two */
static seq_t  *pow2 = NULL;
static size_t  pow2_cnt = 0;
static size_t  pow2_cap = 0;

/* forward declaration */
static void ensure_pow2_up_to(size_t limit);

/* Ensure that all primes ≤ limit (inclusive) are present in the primes array */
static void ensure_primes_up_to(idx_t limit) {
    if ((idx_t)limit <= primes_limit)
        return;

    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
        primes_limit = 1;
    }

    for (uint64_t cand = primes_limit + 1; cand <= (uint64_t)limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = cand;
        }
    }
    primes_limit = limit;

    /* make sure powers of two up to primes_cnt are ready */
    ensure_pow2_up_to(primes_cnt);
}

/* Ensure that pow2[i] = 2^i for all 0 ≤ i ≤ limit */
static void ensure_pow2_up_to(size_t limit) {
    if (pow2_cnt > limit)
        return;

    if (!pow2) {
        pow2_cap = 64;
        pow2 = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        pow2_cnt = 0;
    }

    if (limit + 1 > pow2_cap) {
        size_t new_cap = pow2_cap;
        while (new_cap <= limit)
            new_cap <<= 1;
        pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        pow2_cap = new_cap;
    }

    for (size_t i = pow2_cnt; i <= limit; ++i) {
        if (i == 0)
            pow2[0] = (seq_t)1;
        else
            pow2[i] = pow2[i - 1] * (seq_t)2;
    }
    pow2_cnt = limit + 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;   /* a(1) = 0 by definition */

    /* Ensure primes up to n for factorisation */
    ensure_primes_up_to(n);

    seq_t result = (seq_t)0;
    idx_t rem = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > rem / p)
            break;
        if (rem % p == 0) {
            /* π(p) = i+1  (since primes array is 0‑based) */
            result += pow2[i + 1];
            while (rem % p == 0)
                rem /= p;
        }
    }

    if (rem > 1) {  /* remaining prime factor greater than sqrt(n) */
        ensure_primes_up_to(rem);
        /* binary search for rem in the primes array */
        size_t lo = 0, hi = primes_cnt;
        size_t pos = primes_cnt;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            uint64_t v = primes[mid];
            if (v == rem) {
                pos = mid;
                break;
            } else if (v < rem) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (pos != primes_cnt)
            result += pow2[pos + 1];
    }

    return result;
}
