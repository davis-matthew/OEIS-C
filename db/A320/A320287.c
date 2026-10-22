/*
 * A320287  a(n) = n! * [x^n] Sum_{k>=0} exp(n^k*x)*x^k/k!.
 * https://oeis.org/A320287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * n^(k*(n-k))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using binomial and big_pow utilities */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t nseq = (seq_t)n;          /* index converted to seq_t */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);                 /* binomial(n,k)   */
        seq_t exp = (seq_t)k * (seq_t)(n - k);      /* k*(n-k)         */
        seq_t pow = big_pow(nseq, exp);             /* n^exp           */
        sum += bin * pow;                           /* add term to sum */
    }
    return sum;
}
