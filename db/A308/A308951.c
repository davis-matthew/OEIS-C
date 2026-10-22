/*
 * A308951  Number of principal reversible magic squares of order 4n.
 * https://oeis.org/A308951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{v=0..S} W_v(N) * (W_v(N)+W_{v+1}(N)) * where * N =
 *            4*n, * N = ∏ p_g^{s_g} (prime factorisation), * S = ∑ s_g, *
 *            W_v(N) = Σ_{i=0}^{v} (-1)^{v+i} * binom(v+1,i+1) * * ∏_{g}
 *            binom(s_g+i,i), * W_{S+1}(N) = 0. * a(0) = 1. * * The routine
 *            uses the helper `binom(idx_t n, idx_t k)` to compute...
 * Range:     u8 n <= 3, u16 n <= 59, u32 n <= 2879, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* N = 4*n */
    idx_t N = 4 * n;

    /* factorise N, store exponents s_g in exps[] */
    idx_t tmp = N;
    idx_t exps[64];          /* enough for N <= 4*10^4 */
    size_t   mp = 0;         /* number of distinct prime factors */

    /* factor 2 */
    size_t cnt = 0;
    while (tmp % 2 == 0) {  /* tmp > 0 */
        tmp /= 2;
        ++cnt;
    }
    if (cnt)
        exps[mp++] = (idx_t)cnt;

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        if (cnt)
            exps[mp++] = (idx_t)cnt;
    }
    if (tmp > 1)
        exps[mp++] = 1;   /* remaining prime */

    /* compute S = sum of exponents, k = number of distinct primes */
    idx_t S = 0;
    for (size_t i = 0; i < mp; ++i) S += exps[i];
    idx_t k = (idx_t)mp;   /* not used explicitly, but kept for clarity */

    /* allocate array for W[0..S] (W[S+1] implicitly 0) */
    size_t wsize = (size_t)S + 2;
    seq_t *W = (seq_t *)calloc(wsize, sizeof(seq_t));
    if (!W) return (seq_t)0;   /* out‑of‑memory guard (unlikely) */

    /* compute W[0..S] */
    for (size_t v = 0; v <= (size_t)S; ++v) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        for (size_t i = 0; i <= v; ++i) {
            /* product over all prime exponents */
            seq_t prod = (seq_t)1;
            for (size_t j = 0; j < mp; ++j) {
                prod *= binom(exps[j] + i, i);
            }

            seq_t term = binom(v + 1, i + 1) * prod;

            if (((v + i) & 1) == 0)
                pos += term;
            else
                neg += term;
        }

        /* pos ≥ neg, so subtraction is safe */
        W[v] = pos - neg;
    }
    /* W[S+1] is already zero from calloc */

    /* final sum */
    seq_t result = (seq_t)0;
    for (size_t v = 0; v <= (size_t)S; ++v) {
        result += W[v] * (W[v] + W[v + 1]);
    }

    free(W);
    return result;
}
