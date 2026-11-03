/*
 * A258847  Sum of the k-th pi-based arithmetic derivative of n-k for k=0..n.
 * https://oeis.org/A258847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A258850(n-k,k), where A258850(m,k) is the
 *            k‑th pi‑based arithmetic derivative of m
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* helper: number of primes <= p (p is prime or not). */
static seq_t pi_of_prime(uint64_t p, uint64_t *primes, size_t prime_cnt)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* pi‑based arithmetic derivative, for val > 1. */
static seq_t pi_derivative(seq_t val, uint64_t *primes, size_t prime_cnt)
{
    if (val <= 1)
        return 0;

    seq_t res = 0;
    seq_t temp = val;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t sp = (seq_t)p;

        /* stop when p > sqrt(temp) */
        if ((temp / sp) < sp)
            break;

        if ((temp % sp) == 0) {
            uint64_t e = 0;
            while ((temp % sp) == 0) {
                temp /= sp;
                ++e;
            }
            seq_t d_p = pi_of_prime(p, primes, prime_cnt);
            seq_t term = (seq_t)e * d_p * (val / sp);
            res += term;
        }
    }

    if (temp > 1) {           /* remaining prime factor */
        uint64_t p = (uint64_t)temp;
        seq_t d_p = pi_of_prime(p, primes, prime_cnt);
        seq_t term = d_p * (val / temp); /* e = 1 */
        res += term;
    }
    return res;
}
/* required utility prototypes */
extern void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out);

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Build a prime list large enough for all factors and pi(p) needed. */
    const idx_t prime_limit = 10000000;   /* 10⁷, enough for n ≤ 10⁴ */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(prime_limit, &primes, &prime_cnt);

    seq_t answer = 0;

    for (idx_t m = 0; m <= n; ++m) {
        seq_t val = (seq_t)m;      /* 0‑th derivative */
        idx_t offset = 0;

        while (true) {
            idx_t idx = m + offset;
            if (idx > n)
                break;

            if (idx == n)
                answer += val;

            if (val <= 1)
                break;                 /* further derivatives will be zero */

            seq_t next = pi_derivative(val, primes, prime_cnt);
            if (next == 0)
                break;

            val = next;
            ++offset;
        }
    }

    free(primes);
    return answer;
}
