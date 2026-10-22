/*
 * A335324  Square part of 4th-power-free part of n.
 * https://oeis.org/A335324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = square part of the 4th‑power‑free part of n (OEIS
 *            A335324). For a prime factor p^e in n, keep p^2 if e%4 is 2 or
 *            3; otherwise discard p
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions and types are provided externally:
   seq_t, idx_t.
   The code below uses only basic operators, loops, and no external
   library functions other than standard C headers. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset: a(1) corresponds to n == 1. */
    if (n <= 1) return (seq_t)1;

    /* Temporary variable for factorization. */
    uint64_t tmp = (uint64_t)n;

    seq_t result = (seq_t)1;

    /* Factor out 2 separately. */
    int exp = 0;
    while ((tmp & 1ULL) == 0) {        /* tmp % 2 == 0 */
        tmp >>= 1;                    /* tmp /= 2 */
        exp++;
    }
    if (exp % 4 == 2 || exp % 4 == 3) {
        result *= (seq_t)4;           /* 2^2 */
    }

    /* Factor odd primes up to sqrt(tmp). */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp % 4 == 2 || exp % 4 == 3) {
                result *= (seq_t)p * (seq_t)p;
            }
        }
    }

    /* If remaining tmp is > 1, it's a prime > sqrt(original n). */
    if (tmp > 1) {
        /* The exponent of this prime is 1, so e % 4 == 1 → no factor. */
    }

    return result;
}
