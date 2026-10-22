/*
 * A386047  Primes having only {0, 2, 4, 7} as digits.
 * https://oeis.org/A386047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386047: primes in base 10 whose digits are only {0,2,4,7} */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Generate the n-th prime satisfying the digit condition.  The function
   keeps a static cache of found primes so that calls can be made in
   any order.  Candidates are generated in increasing numeric order by
   increasing length and by enumerating all combinations of the allowed
   digits with the leading digit non‑zero.  Primality is tested by
   is_prime_big. */
seq_t a(idx_t n)
{
    static bool    initialized = false;
    static seq_t * cache       = NULL;
    static size_t  cache_len   = 0;
    static size_t  cache_cap   = 0;

    /* State of the candidate generator. */
    static idx_t   cur_len     = 1;    /* number of digits */
    static idx_t   cur_index   = 0;    /* index within current length */
    static seq_t   pow4        = 1;    /* 4^(cur_len-1) */

    /* Digit mapping: 0 → 0, 1 → 2, 2 → 4, 3 → 7 */
    static const seq_t digit_map[4] = {0, 2, 4, 7};

    if (!initialized) {
        initialized = true;
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
        cur_len = 1;
        cur_index = 0;
        pow4 = 1;
    }

    while (cache_len < n) {
        /* Total candidates of the current length. */
        seq_t total = (cur_len == 1) ? (seq_t)2 : (seq_t)3 * pow4;
        idx_t total_idx = (idx_t)total;

        /* Current candidate value. */
        seq_t cand;
        if (cur_len == 1) {
            cand = (cur_index == 0) ? (seq_t)2 : (seq_t)7;
        } else {
            seq_t idx_seq  = (seq_t)cur_index;
            seq_t lead_idx = idx_seq / pow4;      /* 0,1,2 */
            seq_t rest_idx = idx_seq % pow4;      /* 0 .. pow4-1 */

            seq_t val = (lead_idx == 0) ? (seq_t)2
                                         : ((lead_idx == 1) ? (seq_t)4 : (seq_t)7);

            /* Append the remaining digits (most significant first). */
            idx_t digit_len = cur_len - 1;
            seq_t factor = pow4 / 4;   /* 4^(digit_len-1), zero if digit_len==0 */
            for (idx_t pos = 0; pos < digit_len; ++pos) {
                seq_t digit = (digit_len == 1) ? rest_idx
                                               : (rest_idx / factor);
                rest_idx = (digit_len == 1) ? 0 : (rest_idx % factor);
                if (digit_len != 1) factor /= 4;
                val = val * (seq_t)10 + digit_map[(int)digit];
            }
            cand = val;
        }

        /* Test primality. */
        if (is_prime_big(cand)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = cand;
        }

        /* Advance to the next candidate. */
        cur_index++;
        if (cur_index == total_idx) {
            cur_len++;
            cur_index = 0;
            pow4 *= (seq_t)4;
        }
    }

    return cache[n - 1];
}
