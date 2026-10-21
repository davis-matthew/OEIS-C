/*
 * A282026  a(n) is the smallest m with gcd(m, 2*n+1) = 1 such that 2*n + 2*m
 *          + 1 is composite.
 * https://oeis.org/A282026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m | gcd(m, 2*n+1)=1 and 2*n + 2*m + 1 is composite }
 * Range:     u8 n <= 2144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n * 2 + 1;       // 2*n + 1
    idx_t m = 1;
    for (;; ++m) {
        seq_t sm = (seq_t)m;
        if (gcd(sm, p) == 1) {
            seq_t q = p + sm * 2;    // 2*n + 2*m + 1
            if (!isPrimeGeneric(q))
                return sm;
        }
    }
}
