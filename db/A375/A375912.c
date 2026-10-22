/*
 * A375912  Primes p such that p*nextprime(p)+1 and p + nextprime(p)+1 are
 *          both perfect squares where nextprime(p) is the smallest prime that
 *          is larger than p.
 * https://oeis.org/A375912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A375912
 *
 * Find the n-th prime p such that:
 *   p * nextprime(p) + 1  is a perfect square
 *   p + nextprime(p) + 1  is a perfect square
 *
 * The function searches primes in order using the provided
 * next_prime(uint64_t) helper and tests the two conditions
 * with 128‑bit arithmetic.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* integer square root for unsigned __int128 */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)-1, mid, res = 0;

    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
            res = mid;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* check if a 128‑bit integer is a perfect square */
static bool is_square_uint128(unsigned __int128 x)
{
    uint64_t r = isqrt_uint128(x);
    return (unsigned __int128)r * r == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range */
    idx_t found = 0;
    uint64_t current = 2;           /* start with the first prime */

    while (true) {
        /* next prime after current */
        uint64_t q = next_prime(current);

        /* test the two conditions */
        unsigned __int128 prod = (unsigned __int128)current * q + 1ULL;
        unsigned __int128 sum  = (unsigned __int128)current + q + 1ULL;

        if (is_square_uint128(prod) && is_square_uint128(sum)) {
            ++found;
            if (found == n) break;
        }

        /* move on to the next prime */
        current = q;
    }
    return (seq_t)current;
}
