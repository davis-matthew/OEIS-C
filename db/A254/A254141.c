/*
 * A254141  The average of a(n) consecutive Fibonacci numbers is never an
 *          integer.
 * https://oeis.org/A254141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(n) is the n-th positive integer k for which the average of any
 *            k consecutive Fibonacci numbers is never an integer
 * Range:     u8 n <= 70, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation detail: a(k) is good iff there is no m with
   k | (F_{m+k+1} - F_{m+1}).  This is checked by computing the
   sequence D_n = F_{n+k+1} - F_{n+1} (mod k); if any D_n == 0 for
   0 <= n < period, k fails.  The Pisano period of modulo k is at
   most 6k, so we simply iterate up to 6k steps.  Fib(n) modulo k is
   obtained with fast doubling.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached values a(1), a(2), ... */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_k = 1;             /* next candidate k to test */

/* Ensure capacity for at least 'need' elements. */
static void ensure_capacity(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Fibonacci pair (F_n, F_{n+1}) modulo 'mod' via fast doubling. */
static void fib_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1) {
    if (n == 0) {
        *fn = 0 % mod;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* a=F_k, b=F_{k+1}  */
    /* Compute F_{2k} and F_{2k+1} */
    uint64_t two_b = (b << 1) % mod;
    uint64_t t1 = ( ( ( (__uint128_t) a * ( (two_b + mod - a) % mod ) ) % mod ) ); /* F_{2k} */
    uint64_t t2 = ( ( (__uint128_t) a * a + (__uint128_t) b * b ) % mod );          /* F_{2k+1} */
    if (n & 1) {
        *fn = t2 % mod;
        *fn1 = (t1 + t2) % mod;
    } else {
        *fn = t1 % mod;
        *fn1 = t2 % mod;
    }
}

/* Return F_n modulo mod. */
static uint64_t fib_mod(uint64_t n, uint64_t mod) {
    uint64_t fn, fn1;
    fib_pair(n, mod, &fn, &fn1);
    return fn;
}

/* Test whether k is good (average never integer). */
static bool is_good(uint64_t k) {
    if (k == 0) return false;               /* not used, safety */
    uint64_t d0 = (fib_mod(k + 1, k) + k - 1) % k; /* D_0 = F_{k+1}-1 */
    if (d0 == 0) return false;
    uint64_t d1 = (fib_mod(k + 2, k) + k - 1) % k; /* D_1 = F_{k+2}-1 */
    if (d1 == 0) return false;

    uint64_t prevprev = d0;
    uint64_t prev = d1;

    /* Iterate up to 6*k steps, the Pisano period bound. */
    uint64_t limit = 6 * k;
    for (uint64_t i = 2; i <= limit; ++i) {
        uint64_t cur = (prev + prevprev) % k;
        if (cur == 0) return false;
        prevprev = prev;
        prev = cur;
    }
    return true;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* offset 1; undefined for n<1 */

    /* Ensure we have computed a(1)..a(n) */
    while (cache_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_k;
        if (is_good(cand)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
        next_k++;
    }
    return cache[n - 1];
}
