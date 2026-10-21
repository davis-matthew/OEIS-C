/*
 * A256177  Primes congruent to {8, 13, 18, 23} mod 25.
 * https://oeis.org/A256177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes congruent to 8,13,18,23 modulo 25 (OEIS A256177) */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *vals   = NULL;
    static size_t vals_cap = 0;
    static size_t vals_len = 0;

    /* Next integer to test (starts after 1) */
    static idx_t next_candidate = 1;

    /* Initialise cache on first call */
    if (!vals) {
        vals_cap = 128;
        vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
    }

    /* Expand cache until it holds at least n primes */
    while (vals_len < n) {
        /* Only candidates in the required residue classes modulo 25 */
        idx_t mod = next_candidate % 25u;
        if (mod == 8 || mod == 13 || mod == 18 || mod == 23) {
            if (is_prime(next_candidate)) {       /* from utils */
                if (vals_len >= vals_cap) {
                    vals_cap <<= 1;           /* double capacity */
                    vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                }
                vals[vals_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return vals[n - 1];
}
