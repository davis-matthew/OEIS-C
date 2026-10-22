/*
 * A319081  Number of primes of the form k^3 + n^2 for 0 <= k <= n.
 * https://oeis.org/A319081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 0 ≤ k ≤ n , k³ + n² is prime }, a(0)=0, a(n)=0 if
 *            n is a cube≠1
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 42, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   int_cube_root, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* Return 0 for cubes other than 1 */
    idx_t c = int_cube_root(n);
    if (c > 1 && c * c * c == n)
        return 0;

    seq_t count = 0;
    seq_t nsq   = (seq_t)n * (seq_t)n;   /* n² */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t kk  = (seq_t)k;
        seq_t val = kk * kk * kk + nsq;   /* k³ + n² */
        if (is_prime_big(val))
            ++count;
    }
    return count;
}
