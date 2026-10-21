/*
 * A285328  a(n) = 1 if n is squarefree (A005117), otherwise a(n) = Max {m < n
 *          | same prime factors as n, ignoring multiplicity}.
 * https://oeis.org/A285328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   if n is squarefree then a(n)=1, otherwise a(n)=max { k < n |
 *            same distinct prime divisors as n }
 * Range:     u8 n <= 67, u16 n <= 5767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void
rec(size_t pos, seq_t prod, uint64_t *primes, size_t cnt,
    seq_t N, seq_t *best)
{
    if (pos == cnt) {
        if (prod < N && prod > *best)
            *best = prod;
        return;
    }
    uint64_t p = primes[pos];
    seq_t cur = prod;
    for (uint64_t e = 1; ; ++e) {
        cur *= p;
        if (cur >= N)
            break;
        rec(pos + 1, cur, primes, cnt, N, best);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t cnt = 0;

    factor_u64(nn, primes_buf, exps_buf, &cnt);

    /* n is squarefree if all exponents are 1 */
    bool squarefree = true;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps_buf[i] > 1U) {
            squarefree = false;
            break;
        }
    }
    if (squarefree)
        return 1;

    seq_t best = 0;
    seq_t N = (seq_t)n;
    rec(0, (seq_t)1, primes_buf, cnt, N, &best);

    /* best should be >0 for non‑squarefree n, but return 1 as safety */
    return best ? best : (seq_t)1;
}
