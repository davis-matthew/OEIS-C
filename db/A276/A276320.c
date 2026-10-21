/*
 * A276320  Least number k such that d(Sum_{j=0..n}{k-j}) =
 *          d(Sum_{j=0..n}{k+j}).
 * https://oeis.org/A276320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k ≥ max(1,n) such that * d( (n+1)*(2*k - n)/2 ) =
 *            d( (n+1)*(2*k + n)/2 ) * where d(x) is the number of positive
 *            divisors of x
 * Range:     u8 n <= 243, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* k should be at least n (to keep all terms non‑negative) and at
       least 1 (for the case n = 0). */
    idx_t k = (n >= 1) ? n : (idx_t)1;

    for (;; ++k) {
        /* The first sum: (n+1)*(2*k - n)/2 */
        int64_t t1 = (int64_t)2 * (int64_t)k - (int64_t)n;  /* > 0 for k >= n */
        unsigned __int128 prod1 = (unsigned __int128)(n + 1) *
                                  (unsigned __int128)t1;
        uint64_t A = (uint64_t)(prod1 >> 1);                /* divide by 2 */

        /* The second sum: (n+1)*(2*k + n)/2 */
        int64_t t2 = (int64_t)2 * (int64_t)k + (int64_t)n;   /* always positive */
        unsigned __int128 prod2 = (unsigned __int128)(n + 1) *
                                  (unsigned __int128)t2;
        uint64_t B = (uint64_t)(prod2 >> 1);                /* divide by 2 */

        if (divisor_count_u64(A) == divisor_count_u64(B))
            return (seq_t)k;          /* k is the least such integer */
    }
}
