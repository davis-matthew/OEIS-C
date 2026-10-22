/*
 * A383539  a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,k)^2 *
 *          binomial(n-2*k,k).
 * https://oeis.org/A383539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n‑k,k)^2 * binomial(n‑2k,k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiplicative binomial computation that keeps intermediate values
   as small as possible.  Assumes seq_t supports +, -, *, / and
   conversions from idx_t.  The result fits in seq_t. */
static inline seq_t binom_mul(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use smaller k for fewer iterations */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* Multiply by (n - k + i) then divide by i, keeping the result
           integer at every step. */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Computes the nth term of OEIS A383539.  The function is safe for
   arbitrary-precision seq_t, but the same implementation works for
   the built‑in integer types. */
seq_t a(idx_t n) {
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;
    for (idx_t k = (idx_t)0; k <= maxk; ++k) {
        seq_t b1 = binom_mul(n - k, k);            /* binomial(n‑k,k) */
        seq_t b2 = binom_mul(n - 2 * k, k);        /* binomial(n‑2k,k) */
        seq_t term = b1 * b1 * b2;                 /* (b1)² * b2 */
        sum += term;
    }
    return sum;
}
