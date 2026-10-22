/*
 * A383777  a(n) is the number of steps that n requires to reach 0 under the
 *          map: x -> 2*x + 1 if x is even; 0 if x = 1; x - lpf(x) otherwise
 *          where lpf(x) is the least prime factor of x. a(n) = -1 if 0 is
 *          never reached.
 * https://oeis.org/A383777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of steps to reach 0 under
   x -> (x even) 2*x+1
         (x = 1) 0
         (x odd>1) x - lpf(x)  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/lpf_u64.h"

seq_t a(idx_t n)
{
    if (n == 0ULL) return 0;
    uint64_t x = (uint64_t)n;
    uint64_t steps = 0ULL;
    while (x != 0ULL) {
        if (x == 1ULL) { steps++; break; }
        if ((x & 1ULL) == 0ULL) {
            /* Even case: x = 2*x + 1 */
            x = x * 2ULL + 1ULL;
        } else {
            /* Odd (>1): x = x - lpf(x) */
            uint64_t l = lpf_u64(x);
            x -= l;
        }
        steps++;
    }
    return (seq_t)steps;
}
