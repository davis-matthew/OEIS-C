/*
 * A331539  a(n) gives the number of primes of form (2*n+1)*2^m + 1 where m
 *          satisfies 2^m <= 2*n+1.
 * https://oeis.org/A331539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m | 0 <= m <= floor(log2(2n+1)), (2n+1)*2^m+1 is prime
 *            }
 * Range:     u8 n <= 7, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_log2_uint64, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    idx_t k = 2 * n + 1;
    seq_t count = 0;
    idx_t max_m = floor_log2_uint64(k);

    for (idx_t m = 0; m <= max_m; ++m) {
        seq_t candidate = (seq_t)k * pow2(m) + 1;
        if (is_prime_big(candidate))
            ++count;
    }

    return count;
}
