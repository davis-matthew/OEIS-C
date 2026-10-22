/*
 * A371837  a(n) = Sum_{k=0..floor(n/3)} n^k * binomial(2*n-3*k-1,n-3*k).
 * https://oeis.org/A371837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} n^k * binomial(2*n-3*k-1, n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t result = 0;
    seq_t power  = 1;                         // n^0
    idx_t max_k  = n / 3;                     // floor(n/3)
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t a = 2*n - 3*k - 1;              // a = 2*n - 3*k - 1
        idx_t b = n - 3*k;                    // b = n - 3*k
        seq_t comb = binomial(a, b);          // binomial(a,b)
        seq_t term = power * comb;            // n^k * binomial(...)
        result += term;                       // accumulate sum
        power *= (seq_t)n;                    // prepare n^{k+1}
    }
    return result;
}
