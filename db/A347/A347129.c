/*
 * A347129  a(n) = A347130(n) / A003557(n), where A347130 is the Dirichlet
 *          convolution of the identity function with the arithmetic
 *          derivative of n.
 * https://oeis.org/A347129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A347130(n) / A003557(n)
 * Range:     u8 n <= 35, u16 n <= 3569, u32 n <= 9277, u64 n <= 9277,
 *            u128 n <= 9277, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347130(n) is the Dirichlet convolution of the identity function with the arithmetic derivative of n. */
/* A003557(n) = n / rad(n), where rad(n) is the product of distinct prime factors of n. */

#include <stdint.h>
#include "../utils/factor_u64.h"

static void rec_div(size_t idx,
                    seq_t curr_e, seq_t curr_d,
                    const uint64_t *primes, const uint64_t *exps,
                    size_t pcnt, uint64_t *e_exp, seq_t *A)
{
    if (idx == pcnt) {
        seq_t der = 0;
        for (size_t j = 0; j < pcnt; j++) {
            if (e_exp[j] > 0) {
                /* derivative for prime p_j: e_exp[j] * (curr_e / p_j) */
                der += (seq_t)e_exp[j] * (curr_e / (seq_t)primes[j]);
            }
        }
        *A += curr_d * der;
        return;
    }

    seq_t pow = 1;
    for (size_t val = 0; val <= exps[idx]; val++) {
        e_exp[idx] = val;
        rec_div(idx + 1, curr_e * pow, curr_d / pow,
                primes, exps, pcnt, e_exp, A);
        if (val < exps[idx]) pow *= (seq_t)primes[idx];
    }
}

static seq_t compute_A347130(idx_t n,
                             const uint64_t *primes,
                             const uint64_t *exps,
                             size_t pcnt)
{
    seq_t A = 0;
    uint64_t e_exp[64] = {0};
    rec_div(0, (seq_t)1, (seq_t)n,
            primes, exps, pcnt, e_exp, &A);
    return A;
}

seq_t a(idx_t n)
{
    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Compute rad(n) = product of distinct primes */
    seq_t rad = 1;
    for (size_t i = 0; i < pcnt; i++) {
        rad *= (seq_t)primes[i];
    }

    /* Compute A347130(n) */
    seq_t A347 = compute_A347130(n, primes, exps, pcnt);

    /* Result: a(n) = A347130(n) * rad(n) / n */
    seq_t result = (A347 * rad) / (seq_t)n;
    return result;
}
