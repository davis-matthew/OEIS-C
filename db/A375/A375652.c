/*
 * A375652  Expansion of e.g.f. exp(2*x + x^2 * exp(x)).
 * https://oeis.org/A375652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+2)^(n-2*k) / (k! *
 *            (n-2*k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t binom = 1;   /* C(n, k) */
    seq_t prod  = 1;   /* Π_{i=1}^{k} (n-2k+i) , 1 for k=0 */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t exp    = n - 2 * k;
        seq_t pow_term = big_pow((seq_t)(k + 2), (seq_t)exp);

        seq_t term = binom;
        term *= prod;
        term *= pow_term;
        sum += term;

        if (k == n / 2) break;        /* finished last k */

        /* Update binom: C(n, k+1) = C(n, k) * (n-k) / (k+1) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update prod for next k:
           prod *= (n-2(k+1)+1)(n-2(k+1)+2) / (n-(k+1)+1) */
        idx_t numer_a = n - 2 * (k + 1) + 1;   /* = n-2k-1 */
        idx_t numer_b = n - 2 * (k + 1) + 2;   /* = n-2k   */
        idx_t drop    = n - (k + 1) + 1;       /* = n-k    */

        prod = prod * (seq_t)numer_a * (seq_t)numer_b / (seq_t)drop;
    }

    return sum;
}
