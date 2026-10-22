/*
 * A360813  Expansion of Sum_{k>=0} ( x / (1 - (k * x)^3) )^k.
 * https://oeis.org/A360813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^(3*k) * binomial(n-2*k-1,k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* The first term k = 0 is always 1.  For k > 0 we only need
       to consider k <= n/3 and n >= 2*k+1 (otherwise binomial would be 0). */
    seq_t result = 0;
    // k = 0 gives 1
    result += 1;

    idx_t max_k = n / 3;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* For k > 0, if n < 2*k+1 the binomial coefficient is 0, so break. */
        if (n < 2*k + 1) break;

        idx_t left = n - 3*k;
        /* left^ (3*k) */
        seq_t p = big_pow((seq_t)left, (seq_t)(3*k));
        /* binomial(n-2*k-1, k) */
        seq_t b = binomial(n - 2*k - 1, k);

        /* term = left^(3*k) * binomial(...) */
        seq_t term = p * b;
        result += term;
    }

    return result;
}
