/*
 * A327413  Minimal prime partition representation of odd integers.
 * https://oeis.org/A327413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n-1 if 2n-1 is prime or n = 1 a(n) = (2n-1)! / (p! *
 *            2^k) where p = largest prime < (2n-1) k = ( (2n-1) - p ) / 2
 *            (formula applies for odd integer m = 2n-1, n ≥ 1)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 47, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t m = ((seq_t)n) * 2 - 1;   /* odd integer corresponding to index n */

    /* If m is prime, the minimal partition is [m] */
    if (is_prime_small(m))
        return m;

    /* Find the greatest prime p < m */
    idx_t cand = (idx_t)m - 1;
    /* Only odd candidates need to be checked, except for 2 */
    if (cand % 2 == 0)
        cand--;                     /* make it odd */
    while (cand >= 3 && !is_prime_small(cand))
        cand -= 2;                   /* skip even numbers */

    seq_t p = (seq_t)cand;
    seq_t k = (m - p) / 2;           /* k is small (≤ 10000 for n ≤ 10000) */

    /* Compute product of integers from p+1 to m */
    seq_t result = (seq_t)1;
    for (idx_t x = (idx_t)p + 1; x <= (idx_t)m; ++x)
        result *= (seq_t)x;

    /* Divide by 2^k to obtain the multinomial */
    result /= pow2((idx_t)k);

    return result;
}
