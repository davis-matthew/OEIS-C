/*
 * A328963  Smallest k such that n = sigma_0(k) - ((bigomega(k)-1)*omega(k)),
 *          where sigma_0 = A000005, omega = A001221, bigomega = A001222.
 * https://oeis.org/A328963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 30, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A328963: smallest k with sigma_0(k) - ((bigomega(k)-1)*omega(k)) = n.
 * The implementation does a depth‑first search over exponent vectors
 * (p1^e1 * p2^e2 * …), generating all k and recording the minimal one for
 * each possible value of the quantity f(k)=sigma_0(k)-((bigomega(k)-1)*omega(k)).
 * This is fast enough for n up to several thousand and complies with the
 * type constraints given in the description.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

/* ------------------------------------------------------------------ */
/* Table of small primes – sufficient for the search range used.       */
static const int primes[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};
static const int prime_count = 20;

/* ------------------------------------------------------------------ */
/* Static memory holding the best (smallest) k for each f value.      */
static seq_t *best_vals = NULL;   /* array indexed by f (1‑based)         */
static idx_t  best_limit = 0;     /* size of best_vals (>=0)             */

/* ------------------------------------------------------------------ */
/* Depth‑first search over exponents.                                 */
static void dfs(int idx, int last_exp,
                seq_t cur_k, seq_t P, seq_t S, int t,
                idx_t limit)
{
    /* Record the current k for the current f value (if any).      */
    if (t > 0) {
        seq_t sub = (S - (seq_t)1) * (seq_t)t;
        seq_t f   = P - sub;
        if (f <= (seq_t)limit) {
            idx_t f_idx = (idx_t)f;
            if (best_vals[f_idx] == (seq_t)0 || cur_k < best_vals[f_idx])
                best_vals[f_idx] = cur_k;
        }
    }

    if (idx >= prime_count) return;

    for (int e = 1; e <= last_exp; ++e) {
        seq_t newP  = seq_mul(P, (seq_t)(e + 1));
        seq_t newS  = S + (seq_t)e;
        int  newt   = t + 1;

        seq_t sub   = (newS - (seq_t)1) * (seq_t)newt;
        seq_t fNew  = newP - sub;
        if (fNew > (seq_t)limit) break;          /* larger e → larger fNew */

        seq_t pow_e = pow_uint(primes[idx], e);
        seq_t newK  = seq_mul(cur_k, pow_e);

        dfs(idx + 1, e, newK, newP, newS, newt, limit);
    }
}

/* ------------------------------------------------------------------ */
/* Precompute best_vals up to the requested limit.                    */
static void precompute(idx_t limit)
{
    /* Allocate array big enough for indices 0…limit.                 */
    best_vals = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!best_vals) abort();
    for (idx_t i = 0; i <= limit; ++i) best_vals[i] = (seq_t)0;

    /* The special case k = 1 gives f = 1.                           */
    best_vals[1] = (seq_t)1;

    /* Start recursion with the first prime.                          */
    for (int e = 1; e <= limit; ++e) {
        seq_t P   = (seq_t)(e + 1);
        seq_t S   = (seq_t)e;
        int  t    = 1;

        seq_t sub  = (S - (seq_t)1) * (seq_t)t;
        seq_t fNew = P - sub;
        if (fNew > (seq_t)limit) break;

        seq_t K    = pow_uint(primes[0], e);
        dfs(1, e, K, P, S, t, limit);
    }

    best_limit = limit;
}

/* ------------------------------------------------------------------ */
/* The required sequence entry point.                                 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* offset 1 */
    if (n > best_limit) precompute(n);              /* compute as needed */
    return best_vals[n];
}
