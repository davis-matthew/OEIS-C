/*
 * A348360  a(n) = Product_{d|n} d^(d'), where ' is the arithmetic derivative.
 * https://oeis.org/A348360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} d^(d')
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any n >= 1.  The arithmetic derivative d' of an integer
   d = ∏ p_i^e_i is d' = ∑ e_i * (d / p_i). The function enumerates all
   divisors of n, computes d', and multiplies d^d' for each divisor. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static void rec(int idx, uint64_t cur_div, int *chosen,
                uint64_t *primes, int *exps, int cnt, seq_t *result)
{
    if (idx == cnt) {
        /* Compute the arithmetic derivative d' of cur_div. */
        seq_t d_prime = 0;
        for (int i = 0; i < cnt; ++i) {
            int e = chosen[i];
            if (e > 0) {
                uint64_t term = cur_div / primes[i];
                d_prime += (seq_t)e * (seq_t)term;
            }
        }
        /* Multiply the current result by cur_div^d_prime. */
        seq_t power = big_pow((seq_t)cur_div, d_prime);
        *result = seq_mul(*result, power);
        return;
    }

    uint64_t p = primes[idx];
    int max_e = exps[idx];
    uint64_t div = cur_div;

    for (int e = 0; e <= max_e; ++e) {
        chosen[idx] = e;
        rec(idx + 1, div, chosen, primes, exps, cnt, result);
        if (e < max_e) {
            div *= p;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    /* Prime factorization of N. */
    uint64_t primes[20];
    uint64_t exps_u[20];
    size_t cnt = 0;
    factor_u64(N, primes, exps_u, &cnt);

    /* Convert exponent array to int for recursion. */
    int exps[20];
    for (size_t i = 0; i < cnt; ++i) {
        exps[i] = (int)exps_u[i];
    }

    int chosen[20] = {0};
    seq_t result = 1;
    rec(0, 1, chosen, primes, exps, (int)cnt, &result);
    return result;
}
