/*
 * A286901  Sum of the numbers from prevprime(n) to n.
 * https://oeis.org/A286901
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{i=prevprime(n)..n} i (n ≥ 3)
 * Range:     u8 n <= 51, u16 n <= 2502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9745 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

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
