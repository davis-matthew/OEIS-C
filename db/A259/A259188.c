/*
 * A259188  Primes p such that p, p^2 and p^3 are distinct-digit numbers.
 * https://oeis.org/A259188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, digit_frequencies, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259188: Primes p such that p, p^2 and p^3 are distinct‑digit numbers
   The function computes the n-th such prime.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

/* Return true if x has all digits distinct (no repeats). */
static bool has_distinct_digits(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    for (int d = 0; d < 10; d++)
        if (freq[d] > 1)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t size = 0, cap = 0;
    static idx_t candidate = 2;                 /* next integer to test */

    if (n == 0) /* undefined, but guard against; choose any valid response */
        return 0;

    /* generate terms until we have at least n terms */
    while (size < n)
    {
        if (is_prime(candidate))
        {
            seq_t p = (seq_t)candidate;
            /* compute p² and p³ using big_pow to avoid overflow */
            seq_t sq  = big_pow(p, (seq_t)2);
            seq_t cub = big_pow(p, (seq_t)3);

            if (has_distinct_digits(p) &&
                has_distinct_digits(sq) &&
                has_distinct_digits(cub))
            {
                if (size == cap)
                {
                    cap = (cap == 0) ? 16 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(buf, cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failed, exit gracefully */
                        exit(1);
                    }
                    buf = tmp;
                }
                buf[size++] = p;
            }
        }
        ++candidate;
    }
    return buf[n - 1];
}
