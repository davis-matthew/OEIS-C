/*
 * A381368  a(n) is the least k > n for which prime(n) + prime(k) is a square.
 * https://oeis.org/A381368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > n such that prime(n) + prime(k) is a perfect
 *            square
 * Range:     u8 n <= 7, u16 n <= 4469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Integer square root for uint64_t, using binary search.
   Returns floor(sqrt(n)).  The value n is guaranteed to fit in uint64_t. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 4294967295ULL;   /* 2^32-1 is the largest possible root for uint64_t */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* high is the floor of sqrt(n) after loop exits */
}

seq_t a(idx_t n)
{
    /* prime(n) and prime(k) return 32‑bit integers; cast them to seq_t. */
    seq_t prime_n = (seq_t)get_prime(n);

    for (idx_t k = n + 1; ; ++k) {
        seq_t prime_k = (seq_t)get_prime(k);
        seq_t sum     = prime_n + prime_k;           /* fits comfortably in seq_t for n ≤ 10000 */
        uint64_t sum_u64 = (uint64_t)sum;            /* assume sum < 2^64, which holds for all required n */
        uint64_t s = isqrt_u64(sum_u64);

        if ((uint64_t)s * (uint64_t)s == sum_u64)
            return (seq_t)k;                         /* found the least k > n with the property */
    }
}
