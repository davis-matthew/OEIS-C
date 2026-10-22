/*
 * A397629  Numbers m such that m mod floor(log_3(m)) = 0.
 * https://oeis.org/A397629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 7701, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth integer m ≥ 3 satisfying m % floor(log_3(m)) == 0 */

#include <stddef.h>
#include "../utils/floor_log3.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0, but not used */

    seq_t m = 3;
    idx_t count = 0;

    while (count < n) {
        seq_t k = floor_log3(m);      /* floor(log₃(m)) */
        if (m % k == 0) {
            count++;
            if (count == n) return m;
        }
        m++;
    }
    return m;   /* unreachable */
}
