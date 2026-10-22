/*
 * A383264  Numbers whose vSPD is not squarefree, where vSPD(n) is the
 *          valuation of the smallest prime divisor for n >= 2.
 * https://oeis.org/A383264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   e_not_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A383264 sequence.
   a(n) is the nth positive integer n ≥ 2 whose smallest prime divisor p
   occurs to a power e in n, and e itself is NOT squarefree
   (i.e. some square of a prime divides e).
   The function searches integers starting at 2 and counts qualifying
   terms until the requested index n is reached.  It works for any
   seq_t, not relying on arithmetic beyond the standard operators. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/e_not_squarefree.h"

/* Return the nth element (offset 1) of A383264. */
seq_t a(idx_t n)
{
    idx_t count = 0;   /* numbers found so far */
    for (idx_t m = 2;; ++m) {
        /* Find the smallest prime divisor p of m. */
        idx_t p = 0;
        if (m % 2 == 0) {
            p = 2;
        } else {
            for (idx_t i = 3; i * i <= m; i += 2) {
                if (m % i == 0) { p = i; break; }
            }
            if (p == 0) p = m;  /* m is prime */
        }

        /* Compute the exponent e of p in m. */
        idx_t temp = m;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* If e is not squarefree, m is a term. */
        if (e_not_squarefree(e)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
