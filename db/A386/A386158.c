/*
 * A386158  Primes having only {2, 4, 7, 9} as digits.
 * https://oeis.org/A386158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 108, u32 n <= 77, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386158: primes having only digits {2,4,7,9} (search‑based implementation) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

/* Allowed digits in decimal representation. */
static const uint8_t dmap[4] = { 2, 4, 7, 9 };

/* Current candidate expressed as indices into dmap.
   cand_inds[0] is the most significant digit. */
static uint8_t cand_inds[64];
static size_t cand_len = 0;

/* Initialise the candidate stack to the first prime candidate 2. */
static void init_candidate(void)
{
    if (seq_cache_len == 0) {
        cand_len = 1;
        cand_inds[0] = 0;            /* digit 2 */
    }
}

/* Convert the current digit indices to the numeric value. */
static seq_t cand_value(void)
{
    seq_t v = 0;
    for (size_t i = 0; i < cand_len; ++i)
        v = v * 10 + (seq_t)dmap[cand_inds[i]];
    return v;
}

/* Increment the candidate to the next number using only digits {2,4,7,9}. */
static void inc_candidate(void)
{
    int pos = (int)cand_len - 1;
    while (pos >= 0) {
        if (cand_inds[pos] == 3) {   /* maximum digit reached – wrap */
            cand_inds[pos] = 0;
            --pos;
        } else {
            ++cand_inds[pos];
            break;
        }
    }
    if (pos < 0) {   /* all digits rolled over – increase length */
        for (int i = (int)cand_len; i > 0; --i)
            cand_inds[i] = cand_inds[i - 1];
        cand_inds[0] = 0;
        ++cand_len;
    }
}

/* Extend the cache until it contains at least n primes. */
static void extend_cache(idx_t n)
{
    init_candidate();
    while (seq_cache_len < (size_t)n) {
        seq_t val = cand_value();
        if (is_prime_big(val))
            append_candidate(&seq_cache, &seq_cache_len, &seq_cache_cap, val);
        inc_candidate();
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset */
    if ((size_t)n > seq_cache_len) extend_cache(n);
    return seq_cache[n - 1];
}
