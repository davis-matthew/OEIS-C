/*
 * A348393  a(n) = Sum_{d|n} (n^d)', where ' is the arithmetic derivative.
 * https://oeis.org/A348393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n^d)', arithmetic derivative
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Compute the arithmetic derivative of n:  n' = Σ e_i * (n / p_i)  */
static seq_t derivative_of_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t n_seq = (seq_t)n;
    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * (n_seq / (seq_t)primes[i]);
        der += term;
    }
    return der;
}

/* Recursively generate all divisors of n and accumulate d * n^(d-1) into *sum */
static void recurse_divisors(size_t idx, uint64_t cur,
                             const uint64_t *pr, const uint64_t *ex,
                             size_t cnt, seq_t n_val, seq_t *sum)
{
    if (idx == cnt) {
        seq_t pow = big_pow(n_val, (idx_t)(cur - 1));
        seq_t term = (seq_t)cur * pow;
        *sum += term;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    uint64_t mult = 1;
    for (uint64_t i = 0; i <= e; ++i) {
        recurse_divisors(idx + 1, cur * mult, pr, ex, cnt, n_val, sum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* Offset is 1, safety for n==0 */
    if (n == 1) return 0;             /* a(1) = 0 */

    uint64_t un = (uint64_t)n;
    seq_t n_val = (seq_t)un;

    /* Derivative of n */
    seq_t n_prime = derivative_of_uint64(un);

    /* Factorize n to generate its divisors */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(un, primes, exps, &cnt);

    seq_t sum_div = 0;
    recurse_divisors(0, 1, primes, exps, cnt, n_val, &sum_div);

    return sum_div * n_prime;
}
