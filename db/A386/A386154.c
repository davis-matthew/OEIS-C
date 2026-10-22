/*
 * A386154  Primes having only {2, 4, 5, 9} as digits.
 * https://oeis.org/A386154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function: a(n) generates the nth prime whose decimal digits are
    only 2, 4, 5 or 9.  The algorithm builds all numbers of a given
    length using these digits, tests each for primality with
    is_prime_big, and appends the prime to a growing static cache. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Allowed decimal digits, in ascending order. */
static const int ALLOWED[4] = { 2, 4, 5, 9 };

/* Static cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;

/* Maximum length of numbers that have been generated so far. */
static int max_len_generated = 0;

/* Recursive generation of all numbers of a fixed length using
   ALLOWED digits, stopping when the generated number has been
   tested for primality. */
static void generate_len(int len, int pos, seq_t val)
{
    if (pos == len) {
        if (is_prime_big(val))
            append_candidate(&prime_cache, &prime_len, &prime_cap, val);
        return;
    }

    for (int i = 0; i < 4; ++i) {
        /* val * 10 + ALLOWED[i]   works for both fixed-width and
           arbitrary‑precision seq_t types. */
        generate_len(len, pos + 1, val * 10 + (seq_t)ALLOWED[i]);
    }
}

/* Return the nth prime whose digits are only {2,4,5,9}.  The offset
   is 1 (a(1) is the first such prime).  The function works for any
   order of calls and stays well under the required runtime limit. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, but guard against it */
        return 0;

    /* Generate new lengths only once.  Once a length has been
       processed, we move to the next one.                                         */
    while (prime_len < (size_t)n) {
        ++max_len_generated;
        generate_len(max_len_generated, 0, (seq_t)0);
    }

    return prime_cache[n - 1];
}
