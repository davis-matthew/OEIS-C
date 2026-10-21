/*
 * A246443  Nonnegative integers k satisfying cos(k)*sin(k+1) > 0.
 * https://oeis.org/A246443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 208, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A246443: 
   Nonnegative integers k such that cos(k) * sin(k+1) > 0.
   The function returns the n-th term (n ≥ 1), 1‑based indexing. */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n) {
    idx_t k = 0;            /* current candidate integer k */
    idx_t count = 0;        /* number of terms found so far */

    while (1) {
        /* Evaluate cos(k) and sin(k+1) using double precision
           (exactly zero never occurs for integer arguments). */
        double ck  = cos((double)k);
        double sk1 = sin((double)(k + 1));

        if (ck * sk1 > 0.0) {      /* product positive → term qualifies */
            count++;
            if (count == n)
                return (seq_t)k;   /* return the n-th qualifying k */
        }
        k++;
    }
}
