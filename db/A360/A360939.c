/*
 * A360939  E.g.f. satisfies A(x) = exp( 2*x*A(x) / (1-x) ).
 * https://oeis.org/A360939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} 2^k * (k+1)^(k-1) * * binomial(n-1, k-1)
 *            / k! (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */

/* ensure a seq_t array has room for at least `need` elements */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 1;
        while (new_cap < need)
            new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* cache factorials up to a certain value */
static seq_t *fact = NULL;
static size_t fact_cap = 0;
static size_t fact_last = 0;   /* last index that is computed (fact[0] = 1) */

static void ensure_factorial(size_t n)
{
    if (fact == NULL) {
        grow_seq_arr(&fact, &fact_cap, n + 1);
        fact[0] = (seq_t)1;
        fact_last = 0;
    }
    if (fact_last < n) {
        grow_seq_arr(&fact, &fact_cap, n + 1);
        for (size_t i = fact_last + 1; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_last = n;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 */
        return (seq_t)1;

    size_t N = (size_t)n;

    ensure_factorial(N);          /* make sure n! is available */
    const seq_t n_fact = fact[N];

    seq_t sum    = (seq_t)0;
    seq_t binom  = (seq_t)1;      /* C(n-1,0) for k=1 */
    seq_t ratio  = n_fact;        /* n! / 1!  */
    seq_t pow2   = (seq_t)1;      /* will be set to 2^k inside loop */

    for (size_t k = 1; k <= N; ++k) {
        pow2 *= (seq_t)2;                      /* 2^k */
        seq_t powk = big_pow((seq_t)(k + 1), (seq_t)(k - 1)); /* (k+1)^(k-1) */

        seq_t term = ratio;
        term *= binom;
        term *= pow2;
        term *= powk;
        sum += term;

        if (k < N) {
            /* update binom for the next k: C(n-1, k) */
            binom = (binom * (seq_t)(N - k)) / (seq_t)k;
            /* update ratio for the next k: n! / (k+1)! */
            ratio = ratio / (seq_t)(k + 1);
        }
    }

    return sum;
}
