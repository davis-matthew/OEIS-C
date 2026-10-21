/*
 * A261628  Least prime p such that n-(p*n'-1) and n+(p*n'-1) are both prime
 *          where n' = (3+(-1)^n)/2, or 0 if no such prime p exists.
 * https://oeis.org/A261628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = least prime p such that 
          n - (p * n' - 1)   and   n + (p * n' - 1) 
   are both prime, where n' = (3 + (-1)^n)/2; 
   return 0 if no such prime exists. 
   For small n this yields the values from the OEIS test set.
*/
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/is_prime_small.h" /* is_prime_small(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; guard against accidental 0 */
        return 0;

    seq_t n_val = (seq_t)n;
    /* n' = 2 if n even, else 1 */
    seq_t nprime = ( (n & 1ULL) ? 1 : 2 );

    /* upper bound for p from the conjecture: sqrt(2n) * log(5n) */
    double ub = sqrt(2.0 * (double)n) * log(5.0 * (double)n);
    uint64_t p_limit = (uint64_t)ceil(ub);
    if (p_limit < 2) p_limit = 2;
    seq_t p_limit_seq = (seq_t)p_limit + 1;   /* make bound inclusive */

    for (seq_t p = 2; p < p_limit_seq; ++p) {
        if (!is_prime_small((uint64_t)p))
            continue;

        /* diff = p * n' - 1 */
        seq_t diff = p * nprime - 1;
        seq_t a_val = n_val - diff;
        seq_t b_val = n_val + diff;

        if (a_val < 2 || b_val < 2)
            continue;

        if (is_prime_big(a_val) && is_prime_big(b_val))
            return (seq_t)p;   /* found the least prime */
    }

    /* No prime found within the bound – return 0. */
    return 0;
}
