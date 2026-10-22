/*
 * A393469  Number of mutual visibility sets in the n-necklace graph.
 * https://oeis.org/A393469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + n*(27*n^2 - 6*n + 5)/2 -
 *            7*n*binomial(floor((3*n-1)/2), 2) + 4*n*Σ_{k=0..floor(n/2)-1} [
 *            (floor(3*n/2)-3*k-3)^2 + (3*n-3*k-4) ] * 3^k (Sequence A393469)
 *            For n < 0 the behaviour is undefined. The function works for any
 *            n >= 0 and returns the correct value for all stored terms and
 *            for...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // 3^n
    seq_t val = pow3(n);

    // n*(27*n^2 - 6*n + 5)/2
    seq_t nn = (seq_t)n;
    seq_t term2 = nn * (( (seq_t)27 * nn * nn  -  (seq_t)6 * nn  +  (seq_t)5 )) / ((seq_t)2);

    // 7*n*binomial(floor((3*n-1)/2), 2)
    idx_t binom_arg;
    if (n == 0)
        binom_arg = 0;          // all terms are zero when n==0
    else
        binom_arg = ( (3 * n) - 1 ) / 2;
    seq_t term3 = (seq_t)7 * nn * binom(binom_arg, 2);

    // 4*n * Σ_{k=0..floor(n/2)-1} ((floor(3*n/2)-3*k-3)^2 + (3*n-3*k-4)) * 3^k
    idx_t maxk = n / 2;                 // floor(n/2)
    seq_t sum = (seq_t)0;
    seq_t floor3n2 = ( (seq_t)3 * nn ) / ((seq_t)2);  // floor(3*n/2)
    seq_t three_n = (seq_t)3 * nn;
    for (idx_t k = 0; k < maxk; ++k) {
        seq_t kk = (seq_t)k;
        seq_t a_term = floor3n2 - (seq_t)3 * kk - (seq_t)3;
        seq_t b_term = three_n - (seq_t)3 * kk - (seq_t)4;
        seq_t inner = a_term * a_term + b_term;
        seq_t pow3_k = pow3(k);
        sum += inner * pow3_k;
    }
    seq_t term4 = (seq_t)4 * nn * sum;

    // Combine all terms
    seq_t result = val + term2 + term4;
    result -= term3;
    return result;
}
