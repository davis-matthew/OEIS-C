/*
 * A353516  The largest proper divisor of the primorial base exp-function,
 *          reduced modulo 4.
 * https://oeis.org/A353516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9248, u16 n <= 9248, u32 n <= 9248, u64 n <= 9248,
 *            u128 n <= 9248, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A353516.
   The sequence is the largest proper divisor of the primorial‑base
   exponentiation of n, reduced modulo 4.  Since the exponent of 2 in
   that value is at most 1, the result is always 1 or 3.  The algorithm
   converts n into its primorial‑base digits, identifies the least
   prime factor, removes it, then computes the remaining odd factor
   modulo 4.  Complexity is O(#digits) ≤ 16 for n < 2⁶⁴. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 1 by definition */
        return (seq_t)1;

    /* first 16 primes – enough for any 64‑bit n */
    static const uint64_t primes[16] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
        13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL
    };

    /* prim[i] = product of primes[0..i-1] (prim[0] = 1) */
    static uint64_t prim[17];
    static int initialized = 0;
    if (!initialized) {
        prim[0] = 1ULL;
        for (int i = 1; i < 17; ++i) {
            if (prim[i-1] > UINT64_MAX / primes[i-1])
                prim[i] = UINT64_MAX;
            else
                prim[i] = prim[i-1] * primes[i-1];
        }
        initialized = 1;
    }

    /* find maximum index where prim[i] <= n */
    int maxIdx = 0;
    for (int i = 1; i < 17; ++i) {
        if (prim[i] <= n)
            maxIdx = i;
        else
            break;
    }

    /* compute primorial‑base digits (exponents of primes) */
    uint8_t digits[16] = {0};
    uint64_t rem = n;
    for (int i = maxIdx; i >= 0; --i) {
        uint64_t d = rem / prim[i];
        rem -= d * prim[i];
        digits[i] = (uint8_t)d;
    }

    /* locate the smallest prime divisor (first non‑zero digit) */
    int min_index = -1;
    for (int i = 0; i <= maxIdx; ++i) {
        if (digits[i]) {
            min_index = i;
            break;
        }
    }

    /* evaluate remaining product modulo 4 */
    uint8_t result = 1;   /* only 1 or 3 will occur */
    for (int i = 0; i <= maxIdx; ++i) {
        uint64_t exp_used = digits[i];
        if (i == min_index)          /* remove one factor of the least prime */
            exp_used -= 1;
        if (exp_used == 0)
            continue;

        uint8_t pmod4 = (uint8_t)(primes[i] & 3ULL);   /* 1 or 3 for odd primes */
        if (pmod4 == 3 && (exp_used & 1ULL))
            result ^= 2;          /* toggle between 1 and 3 */
    }

    return (seq_t)result;
}
