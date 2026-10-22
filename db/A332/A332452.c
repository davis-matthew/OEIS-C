/*
 * A332452  Starting from sigma(n), number of halving steps to reach 1 in
 *          '3x+1' problem, or -1 if this never happens.
 * https://oeis.org/A332452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 8191, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332452:  a(n) = number of halving steps needed in the Collatz
   sequence starting from σ(n), the sum of divisors of n.
   For m = σ(n) iterate the Collatz map
          if m even  →  m = m/2   (count the halving)
          if m odd   →  m = 3m + 1
   until m == 1; return the number of halving operations.
   If the sequence never reaches 1 (theoretically never seen up to 10⁶
   steps) the function returns (seq_t)-1. */

#include "../utils/sigma_of.h"
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence is defined from n=1 */
    seq_t m = sigma_of(n);                    /* starting value σ(n) */
    seq_t steps = 0;
    /* safe guard to avoid infinite loops; 1 million is far beyond
       any known halting step for sum‑of‑divisors values up to 10⁴ */
    const unsigned int MAX_ITER = 1000000U;
    unsigned int iter = 0;
    while (m != (seq_t)1 && iter < MAX_ITER) {
        if ((m & (seq_t)1) == 0) {            /* even → halve */
            m >>= 1;
            ++steps;
        } else {                              /* odd → 3m + 1 */
            m = m * (seq_t)3 + (seq_t)1;
        }
        ++iter;
    }
    if (m != (seq_t)1)                        /* non‑halting case */
        return (seq_t)-1;
    return steps;
}
