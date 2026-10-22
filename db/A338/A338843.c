/*
 * A338843  No prime digit is present in a(n) + a(n+1).
 * https://oeis.org/A338843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            nonnegative integers such that for all n ≥ 2, a(n‑1)+a(n)
 *            contains no prime digit (2, 3, 5, 7). The sequence starts
 *            a(1)=0, a(2)=1, a(3)=3, … as defined in OEIS A338843. The
 *            implementation generates terms on demand while memoizing
 *            previous...
 * Range:     u8 n <= 102, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   has_prime_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_prime_digit_uint64.h"

/* Memoisation storage for the sequence. */
static seq_t *seq_arr       = NULL;   /* array of sequence values   */
static size_t seq_len       = 0;      /* number of values stored    */
static size_t seq_cap       = 0;      /* allocated capacity         */

/* Boolean bitmap indicating which non‑negative integers have already
   appeared in the sequence.  Size is dynamically grown as needed. */
static uint8_t *used_arr     = NULL;   /* 1 = used, 0 = unused */
static size_t used_cap       = 0;      /* allocated capacity */

/* Ensure seq_arr can hold at least n elements. */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);  /* out of memory */
    if (seq_arr) {
        for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq_arr[i];
        free(seq_arr);
    }
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Ensure used_arr can address values up to at least n‑1. */
static void ensure_used_cap(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap < n) new_cap <<= 1;

    uint8_t *new_arr = (uint8_t *)realloc(used_arr, new_cap * sizeof(uint8_t));
    if (!new_arr) exit(EXIT_FAILURE);
    for (size_t i = used_cap; i < new_cap; ++i) new_arr[i] = 0;
    used_arr = new_arr;
    used_cap = new_cap;
}

/* Public function: compute a(n) for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the offset 1 sequence; harmless */

    /* Initialise on the first call. */
    if (seq_arr == NULL) {
        ensure_seq_cap(1);
        seq_arr[0] = (seq_t)0;
        seq_len = 1;
        ensure_used_cap(1);
        used_arr[0] = 1;
    }

    /* Generate terms up to the requested index if necessary. */
    if (seq_len < (size_t)n) {
        for (size_t i = seq_len; i < (size_t)n; ++i) {
            uint64_t cand = 0;
            while (true) {
                /* Grow bitmap if needed for this candidate value. */
                ensure_used_cap((size_t)cand + 1);
                if (used_arr[(size_t)cand]) {
                    ++cand;
                    continue;
                }

                /* Check the prime‑digit condition on the sum. */
                uint64_t sum = (uint64_t)seq_arr[i - 1] + cand;
                if (!has_prime_digit_uint64(sum))
                    break;   /* acceptable candidate found */

                ++cand;
            }

            /* Store the chosen value. */
            ensure_seq_cap(i + 1);
            seq_arr[i] = (seq_t)cand;
            used_arr[(size_t)cand] = 1;
            seq_len = i + 1;
        }
    }

    return seq_arr[n - 1];
}
