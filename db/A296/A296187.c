/*
 * A296187  Yarborough primes that remain Yarborough primes when each of their
 *          digits are replaced by their squares.
 * https://oeis.org/A296187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_len, has_zero_or_one, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for sequence A296187: Find the n‑th
   Yarborough prime that remains a prime when each of its digits
   is replaced by that digit's square. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/has_zero_or_one.h"

/* Replace each decimal digit of n by its square and concatenate. */
static seq_t transform_digits_to_squares(seq_t n)
{
    seq_t result = 0;
    seq_t temp   = n;
    int   len    = digit_len(temp);
    seq_t factor = pow10((idx_t)len - 1);

    while (temp > 0) {
        seq_t d   = temp / factor;      /* most significant digit */
        temp -= d * factor;
        factor /= 10;                   /* move to next digit */

        seq_t sq = d * d;               /* square of the digit */
        seq_t mul = (sq < 10) ? 10 : 100; /* digits of square added */
        result = result * mul + sq;
    }
    return result;
}

/* Persistent cache of discovered terms. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_test = 2;   /* integer to be examined next */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* safe fallback for 0‑based calls */

    /* Extend cache until it contains the n‑th element. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_test;

        /* Candidate must have no 0 or 1 digits and be prime. */
        if (!has_zero_or_one(cand) && is_prime_big(cand)) {
            seq_t tr = transform_digits_to_squares(cand);

            /* Square‑digit transform must also be a Yarborough prime. */
            if (!has_zero_or_one(tr) && is_prime_big(tr)) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
            }
        }
        ++next_test;
    }
    return cache[n - 1];
}
