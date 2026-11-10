/*
 * A390962  The values of A390868(k) when k runs over the integers that are
 *          not exponentially odd.
 * https://oeis.org/A390962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A390868(A072587(n)), where A390868(k) = Sum_{d|k, d
 *            non-bi-unitary} phi(k/d); A072587(n) found by scanning integers
 *            for an even prime exponent
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9676 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A390962_MAXF 16

static bool A390962_has_even_exp(uint64_t v)
{
    for (uint64_t p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            int e = 0;
            while (v % p == 0) { v /= p; ++e; }
            if ((e & 1) == 0) return true;
        }
    }
    return false;
}

static void A390962_enum(int idx, uint64_t d, const uint64_t *primes,
                         const int *exps, int pcnt, uint64_t k,
                         int *ai, seq_t *sum)
{
    if (idx == pcnt) {
        bool non = false;
        for (int j = 0; j < pcnt; ++j) {
            if ((exps[j] & 1) == 0 && ai[j] == exps[j] / 2) { non = true; break; }
        }
        if (non) {
            uint64_t m = k / d;
            seq_t phi = (seq_t)m;
            for (int j = 0; j < pcnt; ++j) {
                if (ai[j] < exps[j]) phi -= phi / (seq_t)primes[j];
            }
            *sum += phi;
        }
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        ai[idx] = e;
        A390962_enum(idx + 1, d * mult, primes, exps, pcnt, k, ai, sum);
        mult *= primes[idx];
    }
}

static seq_t A390962_b(uint64_t k)
{
    uint64_t tmp = k;
    uint64_t primes[A390962_MAXF];
    int exps[A390962_MAXF];
    int pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pcnt] = p; exps[pcnt] = e; ++pcnt;
        }
    }
    if (tmp > 1) { primes[pcnt] = tmp; exps[pcnt] = 1; ++pcnt; }

    seq_t sum = 0;
    int ai[A390962_MAXF] = {0};
    A390962_enum(0, 1, primes, exps, pcnt, k, ai, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    static uint64_t last_n = 0, last_k = 1;
    uint64_t cnt, k;
    if (n == 0) return 0;
    if (last_n != 0 && (uint64_t)n >= last_n) {
        cnt = last_n; k = last_k;
    } else {
        cnt = 0; k = 1;
    }
    while (cnt < (uint64_t)n) {
        ++k;
        if (A390962_has_even_exp(k)) ++cnt;
    }
    last_n = cnt; last_k = k;
    return A390962_b(k);
}
