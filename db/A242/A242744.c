/*
 * A242744  Least number k > 2 such that (k!-n)/(k-n) is an integer.
 * https://oeis.org/A242744
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 2 such that (k! – n) is divisible by (k – n)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 2 by the specification of the sequence. */
    for (idx_t k = 3; k <= 2 * n; ++k) {
        if (k == n)                 /* denominator would be zero */
            continue;

        idx_t mabs = (k > n ? k - n : n - k);   /* |k – n|  > 0 */
        idx_t fact_mod;

        /* If k ≥ mabs, then k! is divisible by mabs, so the remainder is 0. */
        if (k >= mabs)
            fact_mod = 0;
        else {
            fact_mod = 1;
            for (idx_t i = 1; i <= k; ++i)
                fact_mod = (fact_mod * i) % mabs;
        }

        if (fact_mod == n % mabs)
            return (seq_t)k;        /* found the least k */
    }

    /* Fallback: in the unlikely event the bound 2*n fails, continue searching
       unboundedly.  The theoretical guarantee from the OEIS comments ensures
       that the preceding loop will always terminate for n > 4, but this
       keeps the function correct for all 'n' in any build. */
    for (idx_t k = 2 * n + 1;; ++k) {
        if (k == n)
            continue;

        idx_t mabs = (k > n ? k - n : n - k);
        idx_t fact_mod;

        if (k >= mabs)
            fact_mod = 0;
        else {
            fact_mod = 1;
            for (idx_t i = 1; i <= k; ++i)
                fact_mod = (fact_mod * i) % mabs;
        }

        if (fact_mod == n % mabs)
            return (seq_t)k;
    }
}
