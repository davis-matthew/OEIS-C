/*
 * A351242  a(n) = n^3 * Sum_{p|n, p prime} 1/p^3.
 * https://oeis.org/A351242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * Sum_{p|n, p prime} 1/p^3
 * Range:     u8 n <= 11, u16 n <= 77, u32 n <= 2939, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed (not used directly here) */

seq_t a(idx_t n)
{
    /* Return 0 for n = 1 (no prime divisors). */
    if (n <= 1)
        return 0;

    seq_t result = 0;
    const idx_t orig_n = n;
    idx_t temp = n;

    /* Factor the number and process each distinct prime factor. */
    idx_t p = 2;
    while ((unsigned __int128)p * p <= temp) {
        if (temp % p == 0) {
            /* Distinct prime factor found: add its contribution. */
            idx_t t = orig_n / p;
            unsigned __int128 t128 = t;
            unsigned __int128 term128 = t128 * t128 * t128;
            result += (seq_t)term128;

            /* Remove all occurrences of this prime from temp. */
            while (temp % p == 0)
                temp /= p;
        }
        /* Advance to the next candidate divisor: 2 -> 3, then odds. */
        p = (p == 2) ? 3 : p + 2;
    }

    /* If temp > 1, it is a prime factor larger than sqrt(original n). */
    if (temp > 1) {
        idx_t t = orig_n / temp;
        unsigned __int128 t128 = t;
        unsigned __int128 term128 = t128 * t128 * t128;
        result += (seq_t)term128;
    }

    return result;
}
