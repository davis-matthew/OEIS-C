/*
 * A281018  Numbers with a prime number of dots and a prime number of dashes
 *          in their International Morse numeral representation.
 * https://oeis.org/A281018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose Morse dot and dash counts are
 *            both prime (using the standard ITU Morse code for decimal
 *            digits). Result is computed by incremental search and memoised
 *            across calls
 * Range:     u8 n <= 48, u16 n <= 1747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Morse dot and dash counts per decimal digit (0-9). */
static const uint8_t DOT[10] = {0,1,2,3,4,5,4,3,2,1};
static const uint8_t DASH[10] = {5,4,3,2,1,0,1,2,3,4};

/* Prime lookup table for sums up to 2000 (more than enough for the expected ranges). */
static bool prime_table[2001];
static bool prime_table_ready = false;

/* Small helper: builds the prime table once. */
static void build_prime_table(void)
{
    if (prime_table_ready) return;
    for (int i = 0; i <= 2000; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;
    for (int i = 2; i * i <= 2000; ++i) {
        if (prime_table[i]) {
            for (int j = i * i; j <= 2000; j += i) prime_table[j] = false;
        }
    }
    prime_table_ready = true;
}

/* Utility: test if a small integer is prime.  Works for all values up to 2000
   via the table; for larger values falls back to trial division. */
static inline bool is_prime_small(uint32_t x)
{
    if (x <= 2000) return prime_table[x];
    if (x < 2) return false;
    for (uint32_t i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* Memoisation storage for the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t next_candidate = 0;   /* number to start next search from */

/* Append a new value to the cache, growing capacity as necessary. */
static void append_cache(seq_t val)
{
    if (seq_cap == 0) {
        seq_cap = 128;
        seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    } else if (seq_len == seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Core function required by the build. */
seq_t a(idx_t n)
{
    /* Initialise prime table on first call. */
    if (!prime_table_ready) build_prime_table();

    /* If we already have the requested element, return it. */
    if (seq_len >= n) return seq_cache[n - 1];

    /* Iterate over integers, starting from the last candidate checked. */
    for (idx_t num = (next_candidate ? next_candidate : 2); ; ++num) {
        /* Count dots and dashes in the Morse representation of num. */
        idx_t tmp = num;
        uint32_t dot_sum = 0, dash_sum = 0;
        if (tmp == 0) {          /* handle the hypothetical 0 case */
            dot_sum = DOT[0];
            dash_sum = DASH[0];
        } else {
            while (tmp > 0) {
                uint8_t d = (uint8_t)(tmp % 10);
                dot_sum += DOT[d];
                dash_sum += DASH[d];
                tmp /= 10;
            }
        }

        /* Check if both counts are prime. */
        if (is_prime_small(dot_sum) && is_prime_small(dash_sum)) {
            append_cache((seq_t)num);
            if (seq_len >= n) {               /* we have reached the desired index */
                next_candidate = num + 1;     /* prepare next start point */
                return seq_cache[n - 1];
            }
        }
        /* Mark that we have now examined this candidate. */
        next_candidate = num + 1;
    }
}
