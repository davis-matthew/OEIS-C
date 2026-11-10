/*
 * A285330  If n is squarefree, then a(n) = A048675(n), otherwise a(n) =
 *          A285328(n).
 * https://oeis.org/A285330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   If A008683(n) <> 0 (n squarefree), a(n) = A048675(n) = Sum_{p|n}
 *            2^(PrimePi(p)-1); otherwise a(n) = A285328(n) = max { k < n :
 *            rad(k) = rad(n) }, found by a search over products of the
 *            distinct primes of n with positive exponents. a(1) = 0
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 97 terms through n = 3478 (2026-11-08)
 * Helpers:   factor_u64, pow2, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/prime_pi.h"

static void A285330_rec(size_t pos, uint64_t prod, const uint64_t *primes, size_t cnt,
                        uint64_t N, uint64_t *best)
{
    if (pos == cnt) {
        if (prod < N && prod > *best) *best = prod;
        return;
    }
    uint64_t p = primes[pos];
    uint64_t cur = prod;
    for (;;) {
        if (cur > (N - 1) / p) break;
        cur *= p;
        A285330_rec(pos + 1, cur, primes, cnt, N, best);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    bool sqfree = true;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) { sqfree = false; break; }
    }

    if (sqfree) {
        seq_t sum = 0;
        for (size_t i = 0; i < cnt; ++i) {
            idx_t pi = (idx_t)prime_pi((idx_t)primes[i]);
            sum += pow2(pi - 1);
        }
        return sum;
    }

    uint64_t best = 0;
    A285330_rec(0, 1, primes, cnt, (uint64_t)n, &best);
    if (best == 0) best = 1;
    return (seq_t)best;
}
