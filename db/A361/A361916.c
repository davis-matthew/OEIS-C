/*
 * A361916  a(n) = n! * Sum_{k=0..floor(n/2)} (-1)^k * (k+1)^(k-1) / (k! *
 *          (n-2*k)!).
 * https://oeis.org/A361916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (-1)^k * (k+1)^(k-1) / (k! (n-2k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Big‑integer helper – pre‑declared in the build environment */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset term */
    if (n == 0)
        return (seq_t)1;

    /*--- 1. Pre‑compute factorials up to 2*n  ---------------------------*/
    static seq_t *fact   = NULL;
    static idx_t fact_cap = 0;           /* largest index currently stored */

    /* ensure fact[0]==1 even if n==1 (first resize occurs here) */
    if (fact_cap == 0 && fact == NULL) {
        fact = (seq_t*)realloc(NULL, sizeof(seq_t) * 1);
        if (!fact) return 0;
        fact[0] = (seq_t)1;
        fact_cap = 0;
    }

    if (n > fact_cap) {
        idx_t need = 2 * n;                /* factorials needed up to 2*n */
        seq_t *tmp = (seq_t*)realloc(fact, sizeof(seq_t) * (need + 1));
        if (!tmp) return 0;                /* allocation failure – abort */
        fact = tmp;

        for (idx_t i = fact_cap + 1; i <= need; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        fact_cap = need;
    }

    /*--- 2. Pre‑compute powers (k+1)^(k-1)  -----------------------------*/
    static seq_t *powk   = NULL;
    static idx_t pow_cap = 0;           /* largest k for which powk is set */

    /* Ensure that powk[0] is defined (1^(-1) is treated as 1) */
    if (pow_cap == 0 && powk == NULL) {
        powk = (seq_t*)realloc(NULL, sizeof(seq_t) * 1);
        if (!powk) return 0;
        powk[0] = (seq_t)1;
        pow_cap = 0;
    }

    idx_t maxk = n / 2;                 /* floor(n/2) */
    if (maxk > pow_cap) {
        seq_t *tmp = (seq_t*)realloc(powk, sizeof(seq_t) * (maxk + 1));
        if (!tmp) return 0;
        powk = tmp;

        for (idx_t k = pow_cap + 1; k <= maxk; ++k) {
            /* big_pow accepts non‑negative exponents – k==0 handled above */
            powk[k] = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
        }
        pow_cap = maxk;
    }

    /*--- 3. Compute a(n) ----------------------------------------------*/
    seq_t sum = (seq_t)0;
    seq_t nf  = fact[n];                /* n! */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = nf;
        term /= fact[n - 2 * k];        /* divide by (n-2k)! */
        term /= fact[k];                /* divide by k! */
        term *= powk[k];                /* multiply by (k+1)^(k-1) */
        if (k & 1)                      /* apply (-1)^k */
            term = -term;
        sum += term;
    }

    return sum;
}
