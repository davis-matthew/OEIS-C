/*
 * A365264  a(n) is the smallest positive integer k whose number of divisors
 *          is larger than that of n.
 * https://oeis.org/A365264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min_{k >= 1, sigma_0(k) > sigma_0(n)} k, where sigma_0(m)
 *            is the number of positive divisors of m
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the routine below uses only the provided utilities and
   no global state.  It works for any build where seq_t and idx_t
   are defined as described in the problem statement. */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/*--------------------------------------------------------------------
 * a(n) – smallest integer k such that d(k) > d(n)
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0 – return 0 as a safe default */
        return (seq_t)0;

    /* divisor count of n (converted to seq_t to keep types uniform)   */
    seq_t sigma_n = (seq_t)divisor_count_u64((uint64_t)n);

    /* search from 1 upward for the first k with a larger divisor count */
    idx_t k = 1;
    for (;; ++k) {
        seq_t sigma_k = (seq_t)divisor_count_u64((uint64_t)k);
        if (sigma_k > sigma_n)
            return (seq_t)k;
    }
}
