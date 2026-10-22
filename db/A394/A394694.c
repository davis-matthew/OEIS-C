/*
 * A394694  Number of occurrences of n in A081493.
 * https://oeis.org/A394694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(n-1)/2 + e(n) where e(n) = 1/2 if n-1 is a
 *            perfect square, 1 if n-1 is a pronic number, 0 otherwise
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/divisor_count_u64.h"   /* uint64_t divisor_count_u64(uint64_t n); */

/* Integer square‑root for 64‑bit values (floor). */
static inline unsigned long long isqrt_u64(unsigned long long n)
{
    unsigned long long low = 0, high = n;
    unsigned long long ans = 0;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if ((mid == 0) && (mid == high)) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1)
        return (seq_t)1;

    /* k = n - 1 (k >= 1 for n > 1). */
    idx_t k = n - 1;
    uint64_t k64 = (uint64_t)k;          /* k fits in 64 bits for our ctor. */

    /* τ(k) – number of divisors of k. */
    seq_t tau = (seq_t)divisor_count_u64(k64);

    /* Detect perfect square. */
    unsigned long long r = isqrt_u64(k64);
    bool is_square = ((unsigned __int128)r * r == (unsigned __int128)k64);

    /* Detect pronic number k = m*(m+1). */
    bool is_pronic = false;
    if (!is_square) {
        unsigned __int128 prod1 = (unsigned __int128)r * (r + 1);
        if (prod1 == k64)
            is_pronic = true;
        else if (r > 0) {
            unsigned __int128 prod0 = (unsigned __int128)(r - 1) * r;
            if (prod0 == k64)
                is_pronic = true;
        }
    }

    /* Compute a(n) according to the formula. */
    seq_t result;
    if (is_square)          /* τ is odd → result = (τ+1)/2. */
        result = (tau + 1) / 2;
    else if (is_pronic)     /* τ even → result = τ/2 + 1. */
        result = tau / 2 + 1;
    else                    /* General case. */
        result = tau / 2;

    return result;
}
