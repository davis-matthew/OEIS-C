/*
 * A323077  Number of iterations of map x -> (x - (largest divisor d < x))
 *          needed to reach 1 or a prime, when starting at x = n.
 * https://oeis.org/A323077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> x - (largest divisor d < x)
 *            until x becomes 1 or a prime, starting from x = n, with a(1)=0
 * Range:     u8 n <= 454, u16 n <= 9665, u32 n <= 9665, u64 n <= 9665,
 *            u128 n <= 9665, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    while (true) {
        if (cur <= (seq_t)1) break;                      /* reached 1 */
        if (is_prime((idx_t)cur)) break;                 /* reached a prime */

        /* find the smallest prime factor of cur (>1) */
        idx_t ux = (idx_t)cur;
        idx_t lpf = 0;
        if ((ux % 2) == 0) {
            lpf = 2;
        } else {
            for (idx_t p = 3; p * p <= ux; p += 2) {
                if (ux % p == 0) {
                    lpf = p;
                    break;
                }
            }
        }

        /* safe because cur is composite, so lpf > 0 */
        seq_t div = cur / lpf;          /* largest proper divisor */
        cur = cur - div;                /* next value */
        ++steps;
    }

    return steps;
}
