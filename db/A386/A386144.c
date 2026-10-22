/*
 * A386144  Primes having only {2, 3, 5, 6} as digits.
 * https://oeis.org/A386144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime * whose decimal digits are only 2,3,5,6; *
 *            all multi‑digit candidates end with 3
 * Range:     u8 n <= 7, u16 n <= 90, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static const int DIGITS[4] = {2, 3, 5, 6};

/* Recursively generate all L‑digit numbers that
 * start with the already built prefix `current`
 * and have the form: first digit + (interior digits) + 3   */
static void generate_middle(idx_t pos, idx_t L, seq_t current,
                            seq_t **arr, size_t *len, size_t *cap) {
    if (pos == L - 1) {                    /* last position: append final 3 */
        seq_t val = current * 10 + 3;
        if (is_prime_big(val))
            append_candidate(arr, len, cap, val);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        generate_middle(pos + 1, L,
                        current * 10 + DIGITS[i],
                        arr, len, cap);
    }
}

/* Generate all L‑digit numbers with the required digit set
 * and test them for primality. */
static void generate_length(idx_t L, seq_t **arr, size_t *len,
                            size_t *cap) {
    if (L == 1) {                          /* single‑digit candidates */
        for (int i = 0; i < 4; ++i) {
            seq_t val = DIGITS[i];
            if (is_prime_big(val))
                append_candidate(arr, len, cap, val);
        }
    } else {                                /* multi‑digit candidates */
        for (int i = 0; i < 4; ++i) {
            seq_t first = DIGITS[i];
            generate_middle(1, L, first, arr, len, cap);
        }
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t generated_len = 0;         /* maximum length generated so far */

    while (cache_len < n) {
        ++generated_len;                     /* increase length */
        generate_length(generated_len,
                        &cache, &cache_len, &cache_cap);
    }
    return cache[n - 1];
}
