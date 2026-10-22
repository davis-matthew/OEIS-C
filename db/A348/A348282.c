/*
 * A348282  a(n) = Sum_{d|n, d>1} mu(d')^2.
 * https://oeis.org/A348282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n , d>1 : (d') is squarefree }
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Arithmetic derivative: d' = Σ_{p^k‖d} k * d / p  */

/* Compute number of squarefree arithmetic derivatives of all divisors d>1 of n. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"

static void dfs(size_t pos, size_t cnt,
                const uint64_t *primes, const uint64_t *exps,
                seq_t curr, seq_t *cntsq, uint64_t *ff)
{
    if (pos == cnt) {
        if (curr > (seq_t)1) {                 /* skip divisor 1    */
            seq_t d = 0;
            for (size_t j = 0; j < cnt; ++j) {
                if (ff[j] > 0) {
                    seq_t part = curr / (seq_t)primes[j];
                    d += (seq_t)ff[j] * part;
                }
            }
            if (is_squarefree(d))
                ++(*cntsq);
        }
        return;
    }

    /* iterate the exponent of primes[pos] from 0 .. exps[pos] */
    seq_t mul = 1;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        ff[pos] = e;
        seq_t next = curr * mul;
        dfs(pos + 1, cnt, primes, exps, next, cntsq, ff);
        mul *= (seq_t)primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    /* factorization of n */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    seq_t count = (seq_t)0;
    uint64_t ff[64];
    dfs(0, pf_cnt, primes, exps, (seq_t)1, &count, ff);
    return count;
}
