/*
 * A326070  Numbers k such that A325977(k) has the same sign as A325978(k).
 * https://oeis.org/A326070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k such that A325977(k) =
 *            (A034448(k)+A048250(k))/2 - k and A325978(k) = k - (σ(k) -
 *            (A034448(k)+A048250(k))/2) have the same (non‑zero) sign
 * Range:     u8 n <= 210, u16 n <= 5736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/pow_uint.h"

static uint64_t *primes      = NULL;
static size_t   primes_cap   = 0;
static size_t   primes_cnt   = 0;

static seq_t   *cache        = NULL;
static size_t   cache_cap    = 0;
static size_t   cache_len    = 0;

static uint64_t cur_k = 1;

/* ensure primes array can hold "need" elements */
static void grow_primes(size_t need) {
    if (primes_cap >= need) return;
    size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes      = tmp;
    primes_cap  = new_cap;
}

/* extend primes list up to at least "limit" */
static void ensure_primes_up_to(uint64_t limit) {
    if (primes_cnt > 0 && primes[primes_cnt-1] >= limit) return;
    uint64_t start = primes_cnt == 0 ? 2 : primes[primes_cnt-1] + 1;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool isp = true;
        uint64_t r = (uint64_t)sqrt((long double)cand);
        for (size_t i = 0; i < primes_cnt && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) { isp = false; break; }
        }
        if (isp) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = cand;
        }
    }
}

/* ensure cache array can hold "need" elements */
static void grow_cache(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* test whether k satisfies the sign condition */
static bool satisfies(uint64_t k) {
    uint64_t m   = k;
    seq_t su     = 1;   /* sum of unitary divisors */
    seq_t ssf    = 1;   /* sum of squarefree divisors */
    seq_t sigma  = 1;   /* sum of all divisors */

    uint64_t max_p = (uint64_t)sqrt((long double)m);
    ensure_primes_up_to(max_p);

    for (size_t i = 0; i < primes_cnt && primes[i] <= max_p; ++i) {
        uint64_t p = primes[i];
        if (m % p != 0) continue;
        int e = 0;
        while (m % p == 0) { m /= p; ++e; }

        seq_t p_pow  = pow_uint(p, (idx_t)e);
        su        *= (1 + p_pow);
        ssf       *= (1 + (seq_t)p);

        seq_t sigma_p_num = pow_uint(p, (idx_t)(e + 1));
        seq_t sigma_p     = (sigma_p_num - 1) / (seq_t)(p - 1);
        sigma     *= sigma_p;
    }

    if (m > 1) {               /* remaining prime factor */
        seq_t pp = (seq_t)m;
        su     *= (1 + pp);
        ssf    *= (1 + pp);
        sigma  *= (1 + pp);     /* sigma for a prime is 1+p */
    }

    seq_t A      = (su + ssf) / 2;
    seq_t N      = (seq_t)k;
    seq_t sumAN  = A + N;

    bool pos1 = (A > N);
    bool neg1 = (A < N);
    bool pos2 = (sumAN > sigma);
    bool neg2 = (sumAN < sigma);

    return (pos1 && pos2) || (neg1 && neg2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (cache_len < (size_t)n) {
        if (satisfies(cur_k)) {
            if (cache_len == cache_cap) grow_cache(cache_cap ? cache_cap*2 : 64);
            cache[cache_len++] = (seq_t)cur_k;
        }
        ++cur_k;
    }
    return cache[n-1];
}
