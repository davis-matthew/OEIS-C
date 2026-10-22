/*
 * A357460  Numbers whose number of deficient divisors is equal to their
 *          number of nondeficient divisors.
 * https://oeis.org/A357460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357460:
 * Numbers k such that the count of deficient divisors of k equals the count
 * of non‑deficient divisors of k.
 * Computation is via a sieve: we precompute the sum and count of divisors,
 * determine for every divisor whether it is deficient, then, by convolution,
 * count deficient divisors of each k up to a fixed limit.  All values up to
 * that limit are stored in a dynamic array, and the public function returns
 * the n‑th term of that array.  The limit is chosen large enough to
 * contain many terms (2 000 000 here). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_LIMIT 2000000U   /* Upper bound for sieve */

/* Global dynamic array of terms of the sequence */
static seq_t *seq_array = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static bool   seq_inited = false;

/* Append a value to the dynamic array, expanding if necessary */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 512;
        seq_t *new_arr = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        seq_array = new_arr;
        seq_cap   = new_cap;
    }
    seq_array[seq_len++] = val;
}

/* One‑time computation of the sequence up to SIEVE_LIMIT */
static void init_sequence(void)
{
    /* Allocate tables for sigma (sum of divisors) and tau (count of divisors) */
    uint32_t *sigma = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    uint16_t *tau   = (uint16_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint16_t));
    if (!sigma || !tau) exit(EXIT_FAILURE);

    /* Classic divisor‑sieve: for each i, add it to multiples of i */
    for (size_t i = 1; i <= SIEVE_LIMIT; ++i) {
        for (size_t j = i; j <= SIEVE_LIMIT; j += i) {
            sigma[j] += (uint32_t)i;
            ++tau[j];
        }
    }

    /* Flag whether a divisor d is deficient: sigma[d] < 2*d */
    uint8_t *defict = (uint8_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!defict) exit(EXIT_FAILURE);
    for (size_t d = 1; d <= SIEVE_LIMIT; ++d) {
        defict[d] = (sigma[d] < 2U * (uint32_t)d) ? 1 : 0;
    }

    /* Count deficient divisors for every n via convolution */
    uint16_t *cnt_def = (uint16_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint16_t));
    if (!cnt_def) exit(EXIT_FAILURE);
    for (size_t d = 1; d <= SIEVE_LIMIT; ++d) {
        if (defict[d]) {
            for (size_t m = d; m <= SIEVE_LIMIT; m += d) {
                ++cnt_def[m];
            }
        }
    }

    /* Extract numbers with equal deficient and non‑deficient divisors:
     * cnt_def[n] == tau[n]/2  (since tau[n] is always even) */
    for (size_t n = 1; n <= SIEVE_LIMIT; ++n) {
        if ((uint32_t)cnt_def[n] * 2U == tau[n]) {
            seq_push((seq_t)n);
        }
    }

    /* Free temporary arrays */
    free(sigma);
    free(tau);
    free(defict);
    free(cnt_def);
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (!seq_inited) {
        init_sequence();
        seq_inited = true;
    }
    if (n == (idx_t)0) return (seq_t)0;               /* out of range */
    if ((size_t)n <= seq_len) return seq_array[(size_t)n - 1];
    return (seq_t)0;   /* requested index beyond pre‑computed range */
}
