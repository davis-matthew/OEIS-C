/*
 * A358106  Quotient of the n-th divisible pair, where pairs are ordered first
 *          by sum and then by denominator.
 * https://oeis.org/A358106
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A208460(n)/A027751(n)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A358106.  Pairs (a,b) with a|b are
   ordered by increasing sum s = a+b, then by increasing a.  The
   n-th term (offset 2) equals the quotient b/a. */

#include <stddef.h>   // for size_t

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  The first term a(2) corresponds
       to the pair (1,1).  We count the pairs starting from index 0
       (first pair).  target is the zero‑based index of the desired pair. */
    idx_t target = n - 2;
    idx_t cnt = 0;               // zero‑based position of the current pair

    for (idx_t s = 2;; ++s) {    // s = a + b, starts at 2
        /* For each sum s, all pairs (a, s-a) with a | s and a <= s/2
           are valid.  Enumerate a in increasing order. */
        for (idx_t a = 1; a <= s / 2; ++a) {
            if (s % a == 0) {          // a divides s  ->  a | (s-a)
                if (cnt == target) {   // we've reached the desired term
                    seq_t term = (seq_t)(s / a - 1);  // b/a = (s-a)/a = s/a - 1
                    return term;
                }
                ++cnt;
            }
        }
    }

    /* Unreachable, but kept for completeness. */
    return 0;
}
