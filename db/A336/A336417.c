/*
 * A336417  Number of perfect-power divisors of superprimorials A006939.
 * https://oeis.org/A336417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=2..n} mu(k) * (1 - Prod_{i=1..n} (1 +
 *            floor(i/k)))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   mu_of, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here mu(k) is the Möbius function, Prod is over exponents of the
   superprimorial S_n = ∏_{i=1..n} prime(i)^(n-i+1).  This implementation
   evaluates the product by grouping equal floor(i/k) values to keep
   the complexity O(n log n) and uses the helper functions mu_of() and
   pow_seq() that are provided by the build environment.  All arithmetic
   is performed in the arbitrary‑precision type seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mu_of.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(0) and a(1) are both 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t pos_sum = (seq_t)0;   /* sum for mu(k) = -1 of (P-1)   */
    seq_t neg_sum = (seq_t)0;   /* sum for mu(k) =  1 of (P-1)   */

    for (idx_t k = 2; k <= n; ++k) {
        /* Möbius value for this k */
        int mu = mu_of(k);
        if (mu == 0)
            continue;

        /* Compute  P(k) = ∏_{i=1..n} (1 + floor(i/k)).
           For i < k we have floor(i/k) = 0 and factor 1, so we only
           iterate over c = floor(i/k) ≥ 1.                       */
        seq_t P = (seq_t)1;

        idx_t max_c = n / k;           /* maximum value of floor(i/k) */
        for (idx_t c = 1; c <= max_c; ++c) {
            /* The range of i giving floor(i/k) == c is:
                 i in [c*k , (c+1)*k - 1] ∩ [1, n]               */
            idx_t start = c * k;
            idx_t end   = (c + 1) * k - 1;
            if (end > n)
                end = n;
            idx_t cnt = end - start + 1;          /* quantity of such i */

            seq_t base = (seq_t)(c + 1);          /* factor (1 + c)   */
            seq_t pow  = pow_seq(base, (int)cnt); /* base^cnt          */
            P *= pow;
        }

        /* (P - 1) is always ≥ 0 because P ≥ 1 */
        seq_t diff = P;
        diff -= (seq_t)1;

        if (mu == -1)
            pos_sum += diff;
        else /* mu == +1 */
            neg_sum += diff;
    }

    /* Result: 1 + pos_sum - neg_sum */
    seq_t temp = pos_sum + (seq_t)1;
    seq_t ans;
    if (temp >= neg_sum)
        ans = temp - neg_sum;
    else
        /* This situation cannot occur for the mathematical definition,
           but we provide a safe fallback. */
        ans = (seq_t)0;

    return ans;
}
