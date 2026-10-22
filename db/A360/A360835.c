/*
 * A360835  Expansion of Sum_{k>=0} (k * x)^k / (1 - (k * x)^3)^(k+1).
 * https://oeis.org/A360835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^n * binomial(n-2*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t baseIdx = n - 3 * k;
        seq_t base = (seq_t)baseIdx;

        /* (n-3*k)^n. When base is 0 and n>0 the value is 0. */
        seq_t pown = 0;
        if (base != 0) {
            pown = big_pow(base, (seq_t)n);
        }

        /* binomial(n-2*k, k). 0 <= k <= floor(n/3) guarantees n-2*k >= k. */
        seq_t bin = binomial(n - 2 * k, k);

        sum += pown * bin;
    }

    return sum;
}
