/*
 * A353959  a(n) = Sum_{d|n} d * (-1)^(d'), where d' is the arithmetic
 *          derivative of d (A003415).
 * https://oeis.org/A353959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} d * (-1)^(d'), d' = arithmetic derivative of d
 * Range:     i8 n <= 62, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void dfs(uint64_t idx, seq_t curr, uint64_t cur_exp[], const uint64_t *primes,
                const uint64_t *exps, size_t pc, seq_t *res)
{
    if (idx == pc) {
        /* compute arithmetic derivative d' of curr */
        seq_t deriv = 0;
        for (size_t j = 0; j < pc; ++j) {
            if (cur_exp[j] == 0) continue;
            seq_t term = curr / primes[j] * (seq_t)cur_exp[j];
            deriv += term;
        }
        seq_t sign = (deriv % 2 == 0) ? (seq_t)1 : (seq_t)-1;
        *res += curr * sign;
        return;
    }

    seq_t d = curr;
    cur_exp[idx] = 0;
    /* e = 0 case */
    dfs(idx + 1, d, cur_exp, primes, exps, pc, res);

    /* e ≥ 1 cases */
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        d *= primes[idx];
        cur_exp[idx] = e;
        dfs(idx + 1, d, cur_exp, primes, exps, pc, res);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* factor n into primes*exponents (small array, n ≤ 10000 in tests) */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pc = 0;

    if ((nn & 1) == 0) {
        primes[pc] = 2;
        exps[pc] = 0;
        while ((nn & 1) == 0) { exps[pc]++; nn >>= 1; }
        ++pc;
    }
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            primes[pc] = p;
            exps[pc] = 0;
            while (nn % p == 0) { exps[pc]++; nn /= p; }
            ++pc;
        }
    }
    if (nn > 1) {
        primes[pc] = nn;
        exps[pc] = 1;
        ++pc;
    }

    seq_t result = 0;
    uint64_t cur_exp[32] = {0};

    dfs(0, (seq_t)1, cur_exp, primes, exps, pc, &result);

    return result;
}
