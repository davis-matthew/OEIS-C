/*
 * A260360  The absolute difference between the largest prime factors of
 *          prime(n)-1 and prime(n+1)-1.
 * https://oeis.org/A260360
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A023503(n+1) - A023503(n)) (offset 2)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1U);
    uint64_t lpf_n   = largest_prime_divisor((uint64_t)p_n   - 1ULL);
    uint64_t lpf_np1 = largest_prime_divisor((uint64_t)p_np1 - 1ULL);
    return (lpf_n > lpf_np1 ? (seq_t)(lpf_n - lpf_np1)
                            : (seq_t)(lpf_np1 - lpf_n));
}
