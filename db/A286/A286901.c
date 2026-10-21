/*
 * A286901  Sum of the numbers from prevprime(n) to n.
 * https://oeis.org/A286901
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{i=prevprime(n)..n} i (n ≥ 3)
 * Range:     u8 n <= 51, u16 n <= 2502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* Largest prime strictly less than n */
    idx_t p = prev_prime(n);

    /* Sum of arithmetic sequence from p to n */
    seq_t sum_terms = (seq_t)(p + n);      /* (p + n)   */
    seq_t count     = (seq_t)(n - p + 1);  /* (n - p + 1) */

    /* Compute (p + n) * (n - p + 1) / 2 safely */
    if ((sum_terms & 1) == 0) {           /* sum_terms even */
        sum_terms >>= 1;                  /* divide by 2 */
    } else {                              /* count even */
        count >>= 1;                      /* divide by 2 */
    }
    return sum_terms * count;
}
