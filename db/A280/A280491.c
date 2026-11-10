/*
 * A280491  a(n) = gcd(n,A122111(n)).
 * https://oeis.org/A280491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A122111(n)); A122111(n) is built prime by prime
 *            from the conjugate partition of n's prime indices, and only the
 *            powers of primes that also divide n are kept
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 134 terms through n = 7792 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Growable list of primes, filled in order by trial division. */
static uint64_t *A280491_pr = NULL;
static size_t A280491_sz = 0, A280491_cap = 0;

static void A280491_add_next_prime(void)
{
    uint64_t c;
    if (A280491_sz == 0) c = 2;
    else if (A280491_sz == 1) c = 3;
    else c = A280491_pr[A280491_sz - 1] + 2;
    for (;; c += (c == 2 ? 1 : 2)) {
        bool ok = true;
        for (size_t i = 0; i < A280491_sz; ++i) {
            uint64_t p = A280491_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok) break;
    }
    if (A280491_sz == A280491_cap) {
        A280491_cap = A280491_cap ? A280491_cap * 2 : 64;
        A280491_pr = (uint64_t *)realloc(A280491_pr, A280491_cap * sizeof(uint64_t));
        if (!A280491_pr) abort();
    }
    A280491_pr[A280491_sz++] = c;
}

/* k-th prime, k >= 1 */
static uint64_t A280491_kth_prime(uint64_t k)
{
    while (A280491_sz < k) A280491_add_next_prime();
    return A280491_pr[k - 1];
}

/* index of prime p (p must be prime) */
static uint64_t A280491_prime_index(uint64_t p)
{
    while (A280491_sz == 0 || A280491_pr[A280491_sz - 1] < p) A280491_add_next_prime();
    size_t lo = 0, hi = A280491_sz - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A280491_pr[mid] < p) lo = mid + 1; else hi = mid;
    }
    return (uint64_t)lo + 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* factor n by trial division; primes come out in increasing order */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            primes[pcnt] = d; exps[pcnt] = e; ++pcnt;
        }
    }
    if (m > 1) { primes[pcnt] = m; exps[pcnt] = 1; ++pcnt; }

    uint64_t total = 0;
    for (size_t i = 0; i < pcnt; ++i) total += exps[i];

    uint64_t result = 1;
    uint64_t remaining = total;
    uint64_t last_part = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t current = A280491_prime_index(primes[i]);
        uint64_t length = current - last_part + 1;   /* exponent of q in A122111(n) */
        uint64_t q = A280491_kth_prime(remaining);
        if (q <= (uint64_t)n) {
            uint64_t t = (uint64_t)n, v = 0;
            while (t % q == 0) { t /= q; ++v; }
            uint64_t k = v < length ? v : length;
            for (uint64_t j = 0; j < k; ++j) result *= q;
        }
        remaining -= exps[i];
        last_part = current + 1;
    }
    return (seq_t)result;
}
