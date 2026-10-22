/*
 * A373831  Numbers whose primorial base representation has alternating digit
 *          sum that is a multiple of 3.
 * https://oeis.org/A373831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373831:
   Numbers whose primorial base representation has alternating digit sum divisible by 3.
   The function a(k) returns the k‑th such integer (a(1)=0, a(2)=3, ...). */

#include <stdint.h>
#include <stddef.h>

static const seq_t PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL,
    233ULL, 239ULL, 241ULL, 251ULL, 257ULL
};
static const size_t PRIME_COUNT = sizeof(PRIMES) / sizeof(PRIMES[0]);

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t temp = m;
        int sum = 0;
        int sign = 1;
        size_t i = 0;
        while (temp > 0) {
            /* Work entirely in unsigned integers that fit in idx_t. */
            idx_t base = (idx_t)PRIMES[i];
            idx_t digit = temp % base;
            temp /= base;
            sum += sign * (int)(digit % 3);
            sign = -sign;
            ++i;
            /* The PRIMES array is large enough for all m we will test
               for the required n (n <= 10000). */
        }
        if (sum % 3 == 0) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
    }
}
