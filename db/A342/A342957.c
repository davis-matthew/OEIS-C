/*
 * A342957  a(n) is the least k such that A342956(k) = n.
 * https://oeis.org/A342957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k with A342956(k) = n, where A342956(k) =
 *            bigomega(sopfr(k)). Method: k = m*P with P the largest prime
 *            factor, so sopfr(k) = s means s = sopfr(m)+P. Enumerate all s <=
 *            K with bigomega(s)=n and all small m, keep the minimum
 *            m*(s-sopfr(m)) where s-sopfr(m) is prime. K is doubled until...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t A342957_K, A342957_best, A342957_mmax;
static uint64_t *A342957_sp;

static bool A342957_small_prime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; d++)
        if (p % d == 0) return false;
    return true;
}

static void A342957_process(uint64_t s)
{
    for (uint64_t m = 1; m <= A342957_mmax; m++) {
        uint64_t t = A342957_sp[m];
        if (t + 2 > s) continue;
        uint64_t P = s - t;
        if (m * P >= A342957_best) continue;
        if (is_prime_u64(P)) A342957_best = m * P;
    }
}

static void A342957_gen(uint64_t prod, uint64_t minp, int rem)
{
    if (rem == 0) { A342957_process(prod); return; }
    for (uint64_t p = minp; ; p++) {
        /* check prod * p^rem <= K */
        uint64_t v = prod;
        bool ok = true;
        for (int i = 0; i < rem; i++) {
            if (v > A342957_K / p) { ok = false; break; }
            v *= p;
        }
        if (!ok) break;
        if (!A342957_small_prime(p)) continue;
        A342957_gen(prod * p, p, rem - 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n > 40) return (seq_t)0;
    int nn = (int)n;
    uint64_t K = (uint64_t)1 << (nn + 2);
    for (;;) {
        int omg = 0;
        for (uint64_t x = K; x; x >>= 1) omg++;
        uint64_t mmax = K * (uint64_t)omg / ((uint64_t)1 << nn) + 2;
        A342957_sp = (uint64_t *)malloc((mmax + 1) * sizeof(uint64_t));
        A342957_sp[0] = 0;
        for (uint64_t m = 1; m <= mmax; m++) {
            uint64_t x = m, sum = 0;
            for (uint64_t d = 2; d * d <= x; d++)
                while (x % d == 0) { sum += d; x /= d; }
            if (x > 1) sum += x;
            A342957_sp[m] = sum;
        }
        A342957_K = K;
        A342957_mmax = mmax;
        A342957_best = K + 1;
        A342957_gen(1, 2, nn);
        free(A342957_sp);
        if (A342957_best <= K) return (seq_t)A342957_best;
        K *= 2;
    }
}
