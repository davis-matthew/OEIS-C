/*
 * A358844  Numbers k for which A276086(6*k) == 5 (mod 6), where A276086 is
 *          the primorial base exp-function.
 * https://oeis.org/A358844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A358844
 *  a(n) = the nth integer k for which A276086(6*k) ≡ 5 (mod 6).
 *  In other words, if f(m) = ∏_{i≥0} p_{i+1}^{d_i(m)} where m is written in
 *  primorial base (d_i(m) are the digits) and p_{i+1} is the next prime,
 *  then a(n) collects the k for which f(6*k) ≡ 5 (mod 6).
 *  The implementation searches k incrementally and memoizes the results.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"   /* next_prime(uint64_t) returns the next prime > x */

/* ----- Helper structure for primes and primorials ----- */
typedef struct {
    uint64_t *primes;      /* primes[i]   = i‑th prime (0‑based)            */
    uint64_t *primorials;  /* primorials[i] = ∏_{j=0..i} primes[j]          */
    size_t prime_cnt;
    size_t primorial_cnt;
    size_t prime_cap;
    size_t primorial_cap;
} PrimeData;

static PrimeData pd = {NULL, NULL, 0, 0, 0, 0};

/* Ensure that primorials up to 'limit' are available. */
static void
ensure_primorials_upto(idx_t limit)
{
    /* initialise structures if empty */
    if (pd.primes == NULL) {
        pd.prime_cap = 4;
        pd.primes = (uint64_t *)malloc(pd.prime_cap * sizeof(uint64_t));
        pd.prime_cnt = 1;
        pd.primes[0] = 2;
    }
    if (pd.primorials == NULL) {
        pd.primorial_cap = 4;
        pd.primorials = (uint64_t *)malloc(pd.primorial_cap * sizeof(uint64_t));
        pd.primorial_cnt = 1;
        pd.primorials[0] = 2;          /* 2# */
    }

    /* Grow until the last primorial exceeds 'limit' */
    while (pd.primorials[pd.primorial_cnt - 1] <= limit) {
        uint64_t next_p = next_prime(pd.primes[pd.prime_cnt - 1]);

        /* resize primes array if necessary */
        if (pd.prime_cnt + 1 > pd.prime_cap) {
            pd.prime_cap = pd.prime_cap * 2 + 4;
            pd.primes = (uint64_t *)realloc(pd.primes, pd.prime_cap * sizeof(uint64_t));
        }
        pd.primes[pd.prime_cnt++] = next_p;

        /* compute new primorial */
        uint64_t last = pd.primorials[pd.primorial_cnt - 1];
        uint64_t new_primorial = last > UINT64_MAX / next_p ? UINT64_MAX : last * next_p;

        /* resize primorial array if necessary */
        if (pd.primorial_cnt + 1 > pd.primorial_cap) {
            pd.primorial_cap = pd.primorial_cap * 2 + 4;
            pd.primorials = (uint64_t *)realloc(pd.primorials, pd.primorial_cap * sizeof(uint64_t));
        }
        pd.primorials[pd.primorial_cnt++] = new_primorial;
    }
}

/* Test whether k satisfies the defining condition. */
static bool
cond(idx_t k)
{
    idx_t m = 6 * k;
    ensure_primorials_upto(m);

    /* Only the parity of exponents where the next prime ≡ 5 (mod 6) matters. */
    uint8_t parity = 0;          /* 0 → even, 1 → odd */
    idx_t rem = m;

    /* Iterate over primorials in reverse order. */
    for (size_t i = pd.primorial_cnt; i-- > 0; ) {
        uint64_t weight = pd.primorials[i];
        if (weight > rem)
            continue;            /* this weight does not contribute yet */

        uint64_t q = rem / weight;
        rem -= q * weight;

        /* primes[i+1] is the next prime after the prime used in this weight */
        if ((pd.primes[i + 1] % 6) == 5 && (q & 1ULL))
            parity ^= 1;
    }
    /* We need parity == 1 (odd number of 5‑mod‑6 bases with odd exponent) */
    return parity == 1;
}

/* ----- Main sequence routine ----- */

/* Sequential cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;      /* next k to test */

seq_t a(idx_t n)
{
    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        if (cond(next_k)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
