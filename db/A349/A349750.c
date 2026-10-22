/*
 * A349750  Odd numbers k such that sigma(k) == k (mod 3), where sigma is the
 *          sum of divisors function.
 * https://oeis.org/A349750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   sigma_mod3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349750: odd numbers k such that σ(k) ≡ k (mod 3). */
/* The function generates odd k, checks σ(k) mod 3 by prime factorisation, */
/* and memoises the sequence in a dynamic array so that calls in any order */
/* are efficient. */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/sigma_mod3_uint64.h"

/* Memoisation array for the sequence. */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Grow the array to hold at least one more element. */
static void seq_grow(void) {
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        seq = new_ptr;
        seq_cap = new_cap;
    }
}

/* Append a value to the memoised array. */
static void seq_append(seq_t val) {
    seq_grow();
    seq[seq_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0U;              /* undefined for n < 1 */

    /* Generate terms up to index n − 1 if required. */
    static uint64_t cur_candidate = 1U; /* next odd number to test */

    while (seq_len < n) {
        uint64_t k = cur_candidate;
        unsigned int sigma_mod = sigma_mod3_uint64(k);
        unsigned int k_mod = (unsigned int)(k % 3U);
        if (sigma_mod == k_mod) {
            seq_append((seq_t)k);      /* cast to seq_t (constructor from uint64_t) */
        }
        cur_candidate += 2U;
    }
    return seq[n - 1];
}
