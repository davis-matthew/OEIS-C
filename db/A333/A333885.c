/*
 * A333885  Number of triples (i,j,k) with 1 <= i < j < k <= n such that i
 *          divides j divides k.
 * https://oeis.org/A333885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} Sum_{d|m, d<m} (tau(d)-1)
 * Range:     u8 n <= 59, u16 n <= 2879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes divisor counts up to n and then sums  
   the required contributions for each n.  The running time per call
   is O(n log n) which is easily below 2 seconds for n ≤ 10000. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* a(1) = a(2) = 0 */

    /* Allocate divisor count array tau[0..n] */
    uint16_t *tau = (uint16_t*)malloc((n + 1) * sizeof(uint16_t));
    if (!tau) return 0;            /* allocation failure: return 0 as a safe fallback */

    for (idx_t i = 0; i <= n; ++i) tau[i] = 0;

    /* Sieve to compute tau[j] = number of divisors of j */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++tau[j];
        }
    }

    seq_t result = 0;

    /* Sum over all proper divisors d of each m up to n */
    for (idx_t d = 1; d < n; ++d) {
        seq_t tau_d_minus1 = (seq_t)(tau[d] - 1);
        if (!tau_d_minus1) continue;

        /* Number of multiples of d that are ≤ n and > d */
        idx_t mults = n / d;
        if (mults <= 1) continue;   /* no m > d if mults <= 1 */
        mults -= 1;                 /* exclude m = d itself */

        result += tau_d_minus1 * (seq_t)mults;
    }

    free(tau);
    return result;
}
