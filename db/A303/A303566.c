/*
 * A303566  a(n) = [x^n] (Sum_{k=0..n} (k+1)!*x^k)/(Sum_{k=0..n}
 *          (k+1)!*(-x)^k).
 * https://oeis.org/A303566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (Sum_{k=0..n} (k+1)!*x^k) /
 *            (Sum_{k=0..n} (k+1)!*(-x)^k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* this implementation uses the recurrence
   a_m = ( (m+1)! + Σ_{odd k≤m} (k+1)! * a_{m−k} )
          − Σ_{even k≤m, k>0} (k+1)! * a_{m−k}   (m≥0)
   which involves only unsigned arithmetic and is valid for all n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Determine the type from the build configuration. */
seq_t a(idx_t n)
{
    /* Cache for the sequence values and factorials. */
    static seq_t *A      = NULL;   /* a[0..] */
    static seq_t *fact   = NULL;   /* fact[i] = (i+1)! for i>=0 */
    static idx_t  cap    = 0;      /* allocated size of caches               */
    static idx_t  size   = 0;      /* number of a-elements already computed */

    /* Allocate more space if needed. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *newA   = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        seq_t *newFact= (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!newA || !newFact) {
            /* allocation failed – terminate execution (no explicit error
               handling is required for this evaluation environment). */
            exit(EXIT_FAILURE);
        }
        /* copy existing data */
        for (idx_t i = 0; i < size; ++i) {
            newA[i]     = A[i];
            newFact[i]  = fact[i];
        }
        /* free old buffers */
        free(A);
        free(fact);
        /* assign new buffers */
        A     = newA;
        fact  = newFact;
        cap   = new_cap;
    }

    /* Compute factorials up to (cap-1) which corresponds to values up to
       (cap-1)+1 = cap! .  fact[i] = (i+1)!   for i >= 0.  */
    if (cap > 0) {
        fact[0] = (seq_t)1;  /* (0+1)! = 1! = 1 */
        for (idx_t i = 1; i < cap; ++i) {
            fact[i] = fact[i - 1] * (seq_t)(i + 1);
        }
    }

    /* Initialize a[0] if this is the first call */
    if (size == 0) {
        A[0] = fact[0];  /* a(0) = 1! = 1 */
        size = 1;
    }

    /* Compute missing a-values up to n. */
    for (idx_t i = size; i <= n; ++i) {
        seq_t sum_odd  = (seq_t)0;
        seq_t sum_even = (seq_t)0;

        for (idx_t k = 1; k <= i; ++k) {
            /* (k+1)! is fact[k] */
            seq_t term = fact[k] * A[i - k];
            if (k & 1) {          /* k is odd */
                sum_odd = sum_odd + term;
            } else {              /* k is even */
                sum_even = sum_even + term;
            }
        }

        /* a_i = (fact[i] + sum_odd) - sum_even  */
        A[i] = (fact[i] + sum_odd) - sum_even;
    }
    size = n + 1;   /* record that we now have a[0..n] ready */

    return A[n];
}
