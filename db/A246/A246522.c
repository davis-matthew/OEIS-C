/*
 * A246522  Number A(n,k) of endofunctions on [n] whose cycle lengths are
 *          divisors of k; square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A246522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of endofunctions on [i] whose cycle lengths divide
 *            k, where (i,k) are the antidiagonal coordinates of n. The
 *            function implements the recurrence derived from the EGF exp(
 *            Σ_{d|k} L(x)^d / d ) with L(x) = Σ_{n≥1} n^{n‑1} x^n / n!. It
 *            memoises A(i,k) for 0 ≤ i,k ≤ 141, which suffices f...
 * Range:     u8 n <= 25, u16 n <= 42, u32 n <= 87, u64 n <= 187,
 *            u128 n <= 462, big any n
 * Helpers:   big_pow, diagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/diagonal_coords.h"

/* size needed for i or j when reading 10000 terms along antidiagonals */
#define MAX_N 142

static seq_t fact[MAX_N];
static seq_t binom[MAX_N][MAX_N];
static seq_t A_val[MAX_N][MAX_N];
static bool   A_done[MAX_N][MAX_N];
static bool   init_done = false;

/* initialise factorials and binomial coefficients */
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    fact[0] = 1;
    for (idx_t i = 1; i < MAX_N; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    for (idx_t n = 0; n < MAX_N; ++n) {
        binom[n][0] = binom[n][n] = 1;
        for (idx_t k = 1; k < n; ++k)
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
    }
}

static inline seq_t nCk(idx_t n, idx_t k)
{
    return binom[n][k];
}

/* compute A(i,k) and store it in A_val[i][k] */
static void compute_A(idx_t i, idx_t k)
{
    if (k >= MAX_N || i >= MAX_N) return;      /* out of scope, should not happen */
    if (A_done[i][k]) return;
    init_tables();

    if (k == 0) {                     /* only the empty function */
        A_val[i][k] = (i == 0) ? 1 : 0;
        A_done[i][k] = true;
        return;
    }
    if (i == 0) {                     /* empty set */
        A_val[i][k] = 1;
        A_done[i][k] = true;
        return;
    }

    /* c[m] = number of connected components of size m whose cycle length
       divides k.  */
    seq_t c[MAX_N] = {0};

    for (idx_t m = 1; m <= i; ++m) {
        seq_t sum = 0;
        for (idx_t d = 1; d <= m && d <= k; ++d) {
            if (k % d != 0) continue;           /* cycle length must divide k */

            seq_t term;
            if (d == m) {
                /* a component that is a single cycle of length m */
                term = fact[d-1];                /* (m‑1)! ways to arrange the m nodes */
            } else {
                /* choose the d cycle nodes, arrange them,
                   and attach trees to them */
                seq_t comb = nCk(m, d) * fact[d-1];
                /* number of forests on m nodes with the chosen d as roots */
                seq_t trees = (seq_t)d * big_pow((seq_t)m,
                                                (seq_t)(m - d - 1));
                term = comb * trees;
            }
            sum += term;
        }
        c[m] = sum;
    }

    /* a[n] = number of functions on a set of size n with admissible cycles.
       Standard recurrence for a set of connected components.  */
    seq_t a[MAX_N] = {0};
    a[0] = 1;
    for (idx_t n = 1; n <= i; ++n) {
        seq_t total = 0;
        for (idx_t m = 1; m <= n; ++m) {
            seq_t term = nCk(n-1, m-1) * c[m];
            term = term * a[n-m];
            total += term;
        }
        a[n] = total;
    }

    A_val[i][k] = a[i];
    A_done[i][k] = true;
}

seq_t a(idx_t n)
{
    idx_t i, k;
    diagonal_coords(n, &i, &k);
    if (i >= MAX_N || k >= MAX_N) return 0;  /* should not happen for n ≤ 10000 */
    compute_A(i, k);
    return A_val[i][k];
}
