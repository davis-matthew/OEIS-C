/*
 * A351566  Radix of the second least significant nonzero digit in the
 *          primorial base expansion of n, or 1 if there is no such digit.
 * https://oeis.org/A351566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9638, u16 n <= 9638, u32 n <= 9638, u64 n <= 9638,
 *            u128 n <= 9638, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A351566: radix of the second least significant
   non‑zero digit in the primorial base expansion of n, or 1 if there
   is no such digit. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First few primes, enough for any n up to ~10^4 (and much larger). */
    static const idx_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };
    const size_t PRIME_COUNT = sizeof(primes) / sizeof(primes[0]);

    idx_t t = n;
    int firstPos  = -1;
    int secondPos = -1;
    size_t i = 0;

    while (t > 0 && i < PRIME_COUNT) {
        idx_t p = primes[i];
        idx_t digit = t % p;
        if (digit != 0) {
            if (firstPos == -1) {
                firstPos = (int)i;
            } else {
                secondPos = (int)i;
                break;
            }
        }
        t /= p;
        ++i;
    }

    if (secondPos == -1) {
        return (seq_t)1;
    } else {
        return (seq_t)primes[secondPos];
    }
}
