/*
 * A344107  a(n) = n! * LaguerreL(n, -n+2).
 * https://oeis.org/A344107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * LaguerreL(n, -n+2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the explicit expansion
   a(n) = Σ_{k=0}^{n} C(n,k) * (n-2)^k * Π_{t=k+1}^{n} t
   (valid for all n ≥ 0).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid negative powers. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* For n ≥ 3  ----------------------------------------------------- */
    const seq_t step = (seq_t)(n - 2);          /* n-2  (positive for n ≥ 3) */

    /* Compute (n-2)^n.  */
    seq_t pow_val = 1;
    for (idx_t i = 0; i < n; ++i)
        pow_val *= step;      /* pow_val == (n-2)^n  */

    /* Initial values for the descending loop.  */
    seq_t binom = 1;          /* C(n,n) = 1                     */
    seq_t prod  = 1;          /* Π_{t=n+1}^{n} t = 1             */
    seq_t sum   = 0;          /* accumulator for the series sum  */

    for (idx_t k = n; ; --k) {
        /* term_k = binom(n,k) * (n-2)^k * Π_{t=k+1}^{n} t  */
        seq_t term = binom * pow_val * prod;
        sum += term;

        if (k == 0) break;   /* finished all terms */

        /* Prepare values for the next (k-1) term.  */
        binom = binom * (seq_t)k / (seq_t)(n - k + 1);  /* C(n,k-1)   */
        pow_val /= step;                                 /* (n-2)^{k-1}*/
        prod *= (seq_t)k;                                /* Π_{t=k}^{n} t  */
    }

    return sum;
}
