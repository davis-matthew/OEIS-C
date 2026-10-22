/*
 * A374587  The maximum exponent in the prime factorization of the numbers
 *          that are not coprime to the maximum exponent in their prime
 *          factorization.
 * https://oeis.org/A374587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq_t, max_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374587 – maximum exponent in the prime factorization of numbers
   that are not coprime to that maximum exponent.  The implementation
   scans positive integers m in order, keeps the n‑th number m with
   gcd(m, max_exponent(m)) > 1, and returns its maximum exponent.  */

#include <stdbool.h>
#include "../utils/gcd_seq_t.h"
#include "../utils/max_exponent.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (1) {
        m++;
        seq_t e = max_exponent(m);
        if (e <= 1)            /* gcd(m,1)=1, skip */
            continue;

        if (gcd_seq_t(m, e) > 1) {
            count++;
            if (count == n)
                return e;
        }
    }
}
