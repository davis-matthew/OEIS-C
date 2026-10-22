/*
 * A372690  Numbers k such that k and k+1 are both numbers whose number of
 *          divisors is a power of 2 (A036537).
 * https://oeis.org/A372690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372690: k such that the divisor counts of k and k+1 are both powers of 2.
   The function linearly scans integers, counting those satisfying the condition
   until the n‑th one is found.  It relies only on the utilities
   divisor_count_u64 and is_power_of_two. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) -> seq_t */
#include "../utils/is_power_of_two.h"     /* is_power_of_two(seq_t) -> bool */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    idx_t found = 0;          /* number of terms found so far */
    uint64_t k = 1;           /* candidate k */

    while (true) {
        seq_t d1 = divisor_count_u64(k);
        seq_t d2 = divisor_count_u64(k + 1);

        if (is_power_of_two(d1) && is_power_of_two(d2)) {
            found++;
            if (found == n) {
                return (seq_t)k;   /* found the n-th term */
            }
        }
        k++;
    }
}
