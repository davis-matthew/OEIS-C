/*
 * A319511  Triangle read by rows: T(n,k) is the number of Boolean functions
 *          on n variables having an algebraic degree equal to k (for n >= 0
 *          and 0 <= k <= n).
 * https://oeis.org/A319511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)th entry of the triangle * T(n,k) = number of
 *            Boolean functions on n variables * having algebraic degree
 *            exactly k. This triangle * is read row‑by‑row, i.e. for each n
 *            we output T(n,0), * T(n,1), …, T(n,n). Thus a(n) = T(r,k) where
 *            r and k * are found from the index n by solving...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 23, u64 n <= 31, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies:  big_pow.h – provides seq_t big_pow(seq_t base, seq_t exp)  */
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Forward declaration of helper that computes T(n,k) */
static seq_t compute_T(idx_t n, idx_t k);

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t idx) {
    /* Find the row r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t cum = 0;          /* Number of terms before row r       */
    while ((cum + (r + 1)) <= idx) {
        cum += r + 1;
        ++r;
    }
    /* k is the position within the row */
    idx_t k = idx - cum;
    /* Compute the triangle entry T(r,k) */
    return compute_T(r, k);
}

/* ------------------------------------------------------------------ */
/* Compute T(n,k) by a multiplicative binomial recurrence.  This
 * avoids factorials and works for any size that fits in seq_t.
 * We also use big_pow to evaluate powers of two with arbitrary
 * exponents represented as seq_t.                                  */
static seq_t compute_T(idx_t n, idx_t k) {
    /* Special case: for k==0 the result is 1 by definition */
    if (k == 0) {
        return (seq_t)1;
    }

    /* Binomial recurrence: C(n,0)=1;  C(n,i+1)=C(n,i)*(n-i)/(i+1) */
    seq_t binom = (seq_t)1;   /* C(n,0) */
    seq_t sum   = (seq_t)0;   /* Σ_{i=0}^{k-1} C(n,i) */

    for (idx_t i = 0; i < k; ++i) {
        sum += binom;                    /* add C(n,i) to the sum   */
        binom = (binom * (n - i)) / (i + 1); /* compute C(n,i+1)    */
    }
    /* After the loop binom == C(n,k)  */
    seq_t pow_bin = big_pow((seq_t)2, binom);   /* 2^C(n,k)   */
    seq_t base = pow_bin - (seq_t)1;            /* 2^C(n,k)-1  */
    seq_t pow_sum = big_pow((seq_t)2, sum);     /* 2^Σ_{i<k}C(n,i) */
    return base * pow_sum;
}
