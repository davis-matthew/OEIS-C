/*
 * A361917  a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k * (k+1)^(k-1) / (k! *
 *          (n-3*k)!).
 * https://oeis.org/A361917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (-1)^k * (k+1)^(k-1) / (k! *
 *            (n-3k)!)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"    /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n) {
    seq_t result = 0;          /* running sum of terms */
    seq_t c = 1;               /* current C(k) = n! / (k! * (n-3k)!)  (C(0)=1) */
    const idx_t Kmax = n / 3;  /* floor(n/3) */

    for (idx_t k = 0; k <= Kmax; ++k) {
        /* (k+1)^(k-1), with the convention 1^(-1) = 1 for k=0 */
        seq_t pow_val;
        if (k == 0) {
            pow_val = 1;
        } else {
            pow_val = pow_uint(k + 1, k - 1);
        }

        /* term for current k: (-1)^k * pow_val * c */
        seq_t term = c * pow_val;
        if (k & 1) {
            result -= term;
        } else {
            result += term;
        }

        /* update C(k+1) for next iteration, unless we are at the last k */
        if (k != Kmax) {
            // mult = (n-3k)*(n-3k-1)*(n-3k-2)
            seq_t mult = (seq_t)(n - 3 * k);
            mult *= (seq_t)(n - 3 * k - 1);
            mult *= (seq_t)(n - 3 * k - 2);
            /* C(k+1) = C(k) * mult / (k+1)  – division is exact */
            c = (c * mult) / (k + 1);
        }
    }
    return result;
}
