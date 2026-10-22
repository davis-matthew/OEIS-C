/*
 * A386162  Primes having only {2, 5, 7, 8} as digits.
 * https://oeis.org/A386162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime that uses only the digits 2,5,7,8 (in
 *            decimal). The function generates the primes in increasing order
 *            by enumerating numbers that end in 7 (except for the first
 *            one‑digit primes 2,5,7)
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static const uint8_t DIGITS[4] = {2, 5, 7, 8};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* irrelevant for offset 1 */

    /* first three primes consisting of allowed digits */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    size_t found = 3;                  /* primes already counted */

    for (size_t len = 2; found < n; ++len)
    {
        size_t digits_len = len - 1;    /* number of free digits */
        uint8_t *idx_arr = (uint8_t *)malloc((size_t)digits_len);
        if (!idx_arr) return (seq_t)0; /* allocation failure */

        for (size_t i = 0; i < digits_len; ++i) idx_arr[i] = 0;

        while (1)
        {
            /* build the candidate number */
            seq_t val = 0;
            for (size_t i = 0; i < digits_len; ++i)
                val = val * 10 + DIGITS[idx_arr[i]];

            val = val * 10 + 7;          /* last digit forced to 7 */

            if (isPrimeGeneric(val))
            {
                ++found;
                if (found == n)
                {
                    free(idx_arr);
                    return val;
                }
            }

            /* increment the base‑4 counter */
            long long pos = (long long)digits_len - 1;
            while (pos >= 0 && ++idx_arr[(size_t)pos] == 4)
            {
                idx_arr[(size_t)pos] = 0;
                --pos;
            }
            if (pos < 0) break;          /* all numbers of this length */
        }

        free(idx_arr);
    }

    return (seq_t)0; /* should never be reached */
}
