/*
 * A276192  Numbers n such that there is no twin prime pair between A000217(n)
 *          and A000217(n+1) (n > 0).
 * https://oeis.org/A276192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth integer k ≥ 1 such that there is no twin prime
 *            pair p, p+2 with T_k ≤ p and p+2 < T_{k+1}, where T_k =
 *            k*(k+1)/2
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Return the nth term of OEIS A276192 */
seq_t a(idx_t n)
{
    idx_t   count = 0;       /* how many qualifying numbers found */
    for (idx_t k = 1; ; ++k) {
        /* triangular numbers in seq_t domain */
        seq_t K  = (seq_t)k;
        seq_t T_k   = K * (K + 1) / 2;
        seq_t T_k1  = (seq_t)(k + 1) * (seq_t)(k + 1 + 1) / 2;

        /* interval [T_k, T_k1-2] to scan for twin primes */
        seq_t start = T_k;
        seq_t end   = T_k1 - 2;   /* last possible p */

        bool has_twin = false;

        if (end >= start) {
            /* start with smallest odd p ≥ 3 not less than start */
            seq_t p = start;
            if (p < 3)
                p = 3; 
            if ((p & 1) == 0)
                ++p;    /* make odd */

            for (; p <= end; p += 2) {
                if (is_prime_big(p) && is_prime_big(p + 2)) {
                    has_twin = true;
                    break;
                }
            }
        }

        if (!has_twin) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
