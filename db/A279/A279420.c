/*
 * A279420  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 0.
 * https://oeis.org/A279420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 3515, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9109 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by searching squares of consecutive integers for a square with an odd number of digits
   whose middle digit is 0 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe return */

    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t last_k = 0;

    while (len < (size_t)n)
    {
        last_k++;
        seq_t sq = last_k * last_k;

        int d = num_digits(sq);
        if ((d & 1) != 0)               /* odd number of digits */
        {
            idx_t exp = (idx_t)((d - 1) / 2);
            seq_t tenpow = pow10(exp);
            seq_t middle = (sq / tenpow) % 10;
            if (middle == 0)
                append_candidate(&cache, &len, &cap, last_k);
        }
    }

    return cache[(size_t)n - 1];
}
