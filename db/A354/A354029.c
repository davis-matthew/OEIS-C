/*
 * A354029  a(n) = 1 if either n or n/2 is a prime power of the form 4m+3,
 *          otherwise 0.
 * https://oeis.org/A354029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n or n/2 is a prime power congruent to 3 (mod 4); 0
 *            otherwise
 * Range:     u8 n <= 65539, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 143 terms through n = 9991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Check whether x is a prime power p^k with p ≡ 3 (mod 4) and k odd. */
static bool is_prime_power_4m3(uint64_t x)
{
    if (x < 2) return false;

    uint64_t p = 0;
    if ((x & 1U) == 0) {               /* even number, smallest prime factor is 2 */
        p = 2;
    } else {
        /* trial division to find the smallest prime factor */
        for (uint64_t i = 3; i <= x / i; i += 2) {
            if (x % i == 0) {
                p = i;
                break;
            }
        }
    }
    if (p == 0) p = x;                 /* x is itself prime */

    /* verify that x consists of only this prime factor, count exponent */
    uint64_t tmp = x;
    int k = 0;
    while (tmp % p == 0) {
        tmp /= p;
        ++k;
    }
    if (tmp != 1) return false;        /* more than one distinct prime */
    if (p % 4 != 3) return false;      /* prime must be 3 mod 4 */
    return (k & 1);                    /* exponent must be odd */
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;

    if ((n64 & 1U) == 0) {             /* even n: test n/2 */
        uint64_t half = n64 / 2;
        return (seq_t)(is_prime_power_4m3(half) ? 1 : 0);
    } else {                           /* odd n: test n itself */
        return (seq_t)(is_prime_power_4m3(n64) ? 1 : 0);
    }
}
