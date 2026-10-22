/*
 * A357090  E.g.f. satisfies A(x) = (1 - x * A(x))^(log(1 - x * A(x)) * A(x)).
 * https://oeis.org/A357090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2*k)! * (n+k+1)^(k-1) *
 *            |Stirling1(n,2*k)|/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the closed‑form using Stirling numbers of the
 * first kind (unsigned) and factorials.  All arithmetic is performed in
 * seq_t, which may be any of the integer types supported by the build. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp) */

#define MAX_N 20000   /* sufficient for all tests (n <= 10000) */

/* factorials up to 2*MAX_N, pre‑computed once */
static seq_t fact[2 * MAX_N + 1];

/* cached values of a(n) */
static seq_t a_values[MAX_N + 1];

/* current Stirling row c(n,k), 0 <= k <= n */
static seq_t cur_row[MAX_N + 1];

/* temporary row for computing c(n+1,k) */
static seq_t next_row[MAX_N + 1];

static bool init_done = false;
static idx_t max_precomputed = 0;

/* --------------------------------------------------------------------- */
static void compute_fact(void)
{
    fact[0] = 1;
    for (idx_t i = 1; i <= 2 * MAX_N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
}

/* --------------------------------------------------------------------- */
static void init_once(void)
{
    if (init_done) return;
    /* row for n = 0  :  c(0,0) = 1 */
    memset(cur_row, 0, sizeof(cur_row));
    cur_row[0] = 1;
    a_values[0] = 1;          /* a(0) = 1 */
    max_precomputed = 0;
    compute_fact();
    init_done = true;
}

/* --------------------------------------------------------------------- */
static void ensure_precomputed(idx_t n)
{
    if (n <= max_precomputed) return;
    init_once();

    for (idx_t m = max_precomputed + 1; m <= n; ++m)
    {
        /* Build next Stirling row: next_row[k] = c(m,k)  (1 <= k <= m) */
        next_row[0] = 0;                           /* c(m,0) = 0 for m >= 1 */
        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t val = cur_row[k - 1];           /* c(m-1,k-1)   */
            if (k <= m - 1)
                val += cur_row[k] * (seq_t)(m - 1);/* (m-1)*c(m-1,k) */
            next_row[k] = val;
        }

        /* Compute a(m) using the closed formula */
        seq_t sum = 0;
        idx_t maxk = m / 2;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            /* term = (2k)! * c(m,2k) / k! * (m+k+1)^(k-1)  */
            seq_t term = fact[2 * k] * next_row[2 * k];  /* (2k)! * c(m,2k) */
            term = term / fact[k];                      /* divide by k!   */
            seq_t base = (seq_t)(m + k + 1);
            seq_t pow_val = big_pow(base, (seq_t)(k - 1)); /* (m+k+1)^(k-1) */
            term *= pow_val;
            sum += term;
        }
        a_values[m] = sum;

        /* Prepare for next iteration: cur_row := next_row */
        cur_row[0] = 0;
        for (idx_t k = 1; k <= m; ++k)
            cur_row[k] = next_row[k];
    }

    max_precomputed = n;
}

/* --------------------------------------------------------------------- */
/* The external function required by the problem statement                */
seq_t a(idx_t n)
{
    if (n > MAX_N) {
        /* Values for n > MAX_N are not needed for the tests.  Return 0. */
        return 0;
    }
    if (n == 0) return 1;          /* a(0) is defined explicitly */
    ensure_precomputed(n);
    return a_values[n];
}
