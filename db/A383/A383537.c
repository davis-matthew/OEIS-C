/*
 * A383537  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k,k)^2 *
 *          binomial(n-2*k,k).
 * https://oeis.org/A383537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(n+k,k)^2 * binomial(n-2k,k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binom(n + k, k);      // binomial(n+k, k)
        seq_t t2 = binom(n - 2 * k, k);   // binomial(n-2k, k)
        seq_t prod = t1 * t1;             // square t1
        prod = prod * t2;                 // multiply by t2
        sum = sum + prod;                 // accumulate
    }
    return sum;
}
