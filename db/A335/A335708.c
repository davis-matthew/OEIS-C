/*
 * A335708  a(n) is the number of divisors of the n-th Niven number.
 * https://oeis.org/A335708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = τ(A005349(n)) (number of divisors of the n‑th Niven
 *            number)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    uint64_t m = 0;          /* current integer being tested */
    idx_t seen = 0;          /* how many Niven numbers have been found */

    while (seen < n) {
        ++m;                              /* next integer to test */
        uint64_t tmp = m, sum = 0;
        while (tmp > 0) {                 /* digit sum of m */
            sum += tmp % 10;
            tmp /= 10;
        }
        if (m % sum == 0) {               /* m is a Niven number */
            ++seen;
            if (seen == n) {
                /* return number of divisors of m */
                return (seq_t)divisor_count_u64(m);
            }
        }
    }
    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
