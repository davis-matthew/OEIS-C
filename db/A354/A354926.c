/*
 * A354926  a(n) = 1 if n is a product of three distinct primes, otherwise 0.
 * https://oeis.org/A354926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of 3 distinct primes, otherwise 0
 * Range:     u8 n <= 8918, u16 n <= 8918, u32 n <= 8918, u64 n <= 8918,
 *            u128 n <= 8918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Returns 1 if n is a product of exactly three distinct primes, 0 otherwise. */
seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    /* The smallest number that is a product of three distinct primes is 2*3*5 = 30. */
    if (m < 30) return 0;

    uint64_t tmp = m;
    size_t cnt = 0;

    /* Factor out 2, checking for repeated factor. */
    if ((tmp & 1U) == 0) {
        size_t exp = 0;
        while ((tmp & 1U) == 0) { ++exp; tmp >>= 1; }
        if (exp != 1) return 0;     /* repeated prime factor */
        ++cnt;
    }

    /* Factor odd primes up to sqrt(tmp). */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) { ++exp; tmp /= p; }
            if (exp != 1) return 0; /* repeated prime factor */
            ++cnt;
            if (cnt > 3) return 0;  /* already more than 3 distinct primes */
        }
    }

    /* Remaining tmp > 1 is a prime factor > sqrt(original n). */
    if (tmp > 1) {
        ++cnt;
        if (cnt > 3) return 0;
    }

    return (seq_t)(cnt == 3 ? 1 : 0);
}
