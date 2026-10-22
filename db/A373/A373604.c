/*
 * A373604  a(n) = 1 if the primorial base representation of 6*n has
 *          alternating digit sum (A373605) that is a multiple of 3, otherwise
 *          0.
 * https://oeis.org/A373604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 iff the alternating digit sum of the primorial base
 *            representation of 6*n is a multiple of 3; otherwise 0
 * Range:     u8 n <= 34, u16 n <= 8987, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 6 * n in primorial base. */
    seq_t x = (seq_t)n * 6;

    /* First 15 primes are enough for the range of n we handle. */
    static const uint64_t primes[15] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
        29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL
    };

    /* Build primorial place values up to the first one that exceeds x. */
    seq_t prims[15];
    int i, last;
    seq_t p = 1;
    i = 0;
    while (p <= x && i < 15) {
        prims[i] = p;
        p *= primes[i];
        ++i;
    }
    last = i - 1;

    /* Sum digits in odd/even positions modulo 3. */
    int sum_even = 0, sum_odd = 0;
    int pos = last;
    while (pos >= 0) {
        seq_t digit = x / prims[pos];
        x %= prims[pos];
        int dmod = (int)(digit % 3);
        if (pos & 1) {
            sum_odd += dmod;
            if (sum_odd >= 3) sum_odd -= 3 * (sum_odd / 3);
        } else {
            sum_even += dmod;
            if (sum_even >= 3) sum_even -= 3 * (sum_even / 3);
        }
        --pos;
    }

    return (sum_even % 3 == sum_odd % 3) ? (seq_t)1 : (seq_t)0;
}
