/*
 * A367846  Expansion of e.g.f. 1/(1 - x + log(1 - 3*x)).
 * https://oeis.org/A367846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n * a(n-1) + Σ_{k=1..n} 3^k * (k-1)! *
 *            binomial(n,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_vals  = NULL;     /* a[0..n]                           */
static seq_t *pow3    = NULL;     /* 3^i for i = 0..n                 */
static seq_t *fact    = NULL;     /* i!  for i = 0..n (used as (i-1)!)*/
static size_t max_n   = 0;        /* largest n that has been computed  */

/* Ensure that the arrays cover index n, allocating or growing as needed. */
static void ensure(idx_t n)
{
    /* First time: allocate a minimal array for n = 0.  */
    if (a_vals == NULL) {
        size_t init = 1;                     /* store indices 0 .. 1 */
        a_vals = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        pow3   = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        fact   = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        if (!a_vals || !pow3 || !fact) {
            /* Allocation failed – leave arrays NULL, caller will abort. */
            return;
        }
        a_vals[0] = (seq_t)1;
        pow3[0]   = (seq_t)1;
        fact[0]   = (seq_t)1;
        max_n     = 0;
    }

    size_t need = (size_t)n;
    if (need <= max_n) return;               /* everything already available */

    /* Grow the arrays to accommodate indices up to need.  */
    a_vals = (seq_t *)realloc(a_vals, (need + 1) * sizeof(seq_t));
    pow3   = (seq_t *)realloc(pow3,   (need + 1) * sizeof(seq_t));
    fact   = (seq_t *)realloc(fact,   (need + 1) * sizeof(seq_t));
    if (!a_vals || !pow3 || !fact) {
        /* Allocation failed – leave as is; subsequent calls will fail. */
        return;
    }

    /* Compute powers of 3 and factorials for the new indices.  */
    for (size_t i = max_n + 1; i <= need; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
        fact[i] = fact[i - 1] * (seq_t)i;     /* i!  */
    }

    /* Compute the sequence values for the new indices using the recurrence. */
    for (size_t i = max_n + 1; i <= need; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                /* C(i,0) = 1 */

        for (size_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient: C(i,k) = C(i,k-1) * (i-k+1) / k */
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;

            seq_t term = pow3[k] * fact[k - 1] * binom * a_vals[i - k];
            sum += term;
        }
        a_vals[i] = (seq_t)i * a_vals[i - 1] + sum;
    }

    max_n = need;
}

seq_t a(idx_t n)
{
    ensure(n);
    return a_vals[(size_t)n];
}
