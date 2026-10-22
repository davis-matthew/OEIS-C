/*
 * A319686  Number of distinct values obtained when arithmetic derivative
 *          (A003415) is applied to the divisors of n.
 * https://oeis.org/A319686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of the arithmetic * derivative
 *            A003415 applied to the divisors of n. For n=1 the * only divisor
 *            is 1 whose derivative is 0, so a(1)=1
 * Range:     u8 n <= 95, u16 n <= 9740, u32 n <= 9740, u64 n <= 9740,
 *            u128 n <= 9740, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Factorise 64‑bit integer n.
 * primes[i]  : i‑th distinct prime factor
 * exps[i]    : exponent of that prime factor
 * Returns number of distinct prime factors. */
static int
factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps)
{
    int cnt = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {                 /* remaining prime factor */
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* Recursive enumeration of all divisors of n.
 * cur_exps holds the exponent chosen so far.
 * cur_div   current divisor corresponding to those exponents.
 * derivs    array to accumulate arithmetic derivative values. */
static void
gen_divs(int idx,
         int prime_cnt,
         uint64_t *primes,
         uint64_t *exps,
         uint64_t *cur_exps,
         uint64_t cur_div,
         seq_t *derivs,
         size_t *deriv_idx)
{
    if (idx == prime_cnt) {
        /* arithmetic derivative of cur_div = cur_div * Σ e_i / p_i.
         * Equivalent to Σ e_i * (cur_div / p_i). */
        seq_t d = 0;
        for (int i = 0; i < prime_cnt; ++i) {
            if (cur_exps[i] > 0) {
                d += (seq_t)(cur_div / primes[i]) * (seq_t)cur_exps[i];
            }
        }
        derivs[*deriv_idx] = d;
        ++(*deriv_idx);
        return;
    }

    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        cur_exps[idx] = e;
        uint64_t new_div = cur_div;
        for (uint64_t k = 0; k < e; ++k) new_div *= primes[idx];
        gen_divs(idx + 1, prime_cnt, primes, exps,
                 cur_exps, new_div, derivs, deriv_idx);
    }
}

seq_t a(idx_t n)
{
    /* special case n = 1: divisor 1 has derivative 0. */
    if ((uint64_t)n == 1) return 1;

    uint64_t primes[64], exps[64], cur_exps[64];
    int prime_cnt = factor_uint64((uint64_t)n, primes, exps);

    /* total number of divisors = Π (e_i + 1). */
    size_t div_cnt = 1;
    for (int i = 0; i < prime_cnt; ++i) div_cnt *= (size_t)(exps[i] + 1);

    seq_t *derivs = (seq_t *)malloc(div_cnt * sizeof(seq_t));
    if (!derivs) return 0; /* allocation failure unlikely for small n */

    size_t deriv_idx = 0;
    gen_divs(0, prime_cnt, primes, exps, cur_exps, 1ULL, derivs, &deriv_idx);

    /* Count distinct derivative values. */
    size_t distinct = 0;
    seq_t *distinct_vals = (seq_t *)malloc(div_cnt * sizeof(seq_t));
    for (size_t i = 0; i < div_cnt; ++i) {
        bool unique = true;
        for (size_t j = 0; j < distinct; ++j) {
            if (derivs[i] == distinct_vals[j]) {
                unique = false;
                break;
            }
        }
        if (unique) {
            distinct_vals[distinct++] = derivs[i];
        }
    }

    free(derivs);
    free(distinct_vals);

    return (seq_t)distinct;
}
