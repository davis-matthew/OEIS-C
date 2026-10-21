/*
 * A298975  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 5, and no term occurs twice.
 * https://oeis.org/A298975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the lexicographically first permutation of the positive
 *            integers such that a(n)*a(n+1) has a decimal digit 5 and no term
 *            appears twice. a(1)=1
 * Range:     u8 n <= 7, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   has_digit_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers available for C11 and C++17. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/has_digit_five.h"

/* Dynamic array holding the sequence values.  seq[i] = a(i+1). */
static seq_t *seq = NULL;
static size_t seq_len = 0;   /* number of terms currently stored */
static size_t seq_cap = 0;   /* capacity of the seq array */

/* Bitset marking which numbers have already appeared in the sequence.
   Bit 0 corresponds to integer 1, bit 1 to integer 2, etc. */
static uint64_t *used_bits = NULL;
static size_t used_words = 0;   /* number of uint64_t words allocated */

/* Ensure that used_bits can hold a flag for the value x. */
static void ensure_used_bits_for(seq_t x)
{
    if (x == 0)
        return;
    /* Convert x into an unsigned 64‑bit index.  The sequence will never
       generate values that overflow 64 bits for the tested n. */
    size_t idx  = (size_t)((uint64_t)(x - (seq_t)1));
    size_t word = idx >> 6;           /* divide by 64 */
    if (word >= used_words) {
        size_t new_words = used_words ? used_words * 2 : 64;
        if (new_words < word + 1)
            new_words = word + 1;
        uint64_t *ptr = (uint64_t*)realloc(used_bits,
                                            new_words * sizeof(uint64_t));
        if (!ptr)
            abort();
        /* Zero the newly allocated words. */
        memset(ptr + used_words, 0,
               (new_words - used_words) * sizeof(uint64_t));
        used_bits = ptr;
        used_words = new_words;
    }
}

/* Test whether x has already been used in the sequence. */
static bool used(seq_t x)
{
    if (x == 0)
        return true;   /* protect us from zero */
    size_t idx  = (size_t)((uint64_t)(x - (seq_t)1));
    size_t word = idx >> 6;
    if (word >= used_words)
        return false;   /* not allocated → not used yet */
    size_t bit = idx & 63;
    return ((used_bits[word] >> bit) & 1ULL) != 0;
}

/* Mark the integer x as used. */
static void mark_used(seq_t x)
{
    if (x == 0)
        return;
    ensure_used_bits_for(x);
    size_t idx  = (size_t)((uint64_t)(x - (seq_t)1));
    size_t word = idx >> 6;
    size_t bit  = idx & 63;
    used_bits[word] |= (1ULL << bit);
}

/* Public function: reach the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of bounds – not used by the tests */

    /* Grow the sequence to include a(n). */
    while (seq_len < (size_t)n) {
        seq_t prev = 0;
        if (seq_len > 0)
            prev = seq[seq_len - 1];

        /* Special case: the first term is defined as 1. */
        if (seq_len == 0) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)1;
            mark_used((seq_t)1);
            continue;
        }

        /* Find the smallest unused integer that satisfies the digit‑5
           product condition. */
        seq_t cand = (seq_t)1;
        for (;;) {
            if (!used(cand)) {
                seq_t prod = prev * cand;
                if (has_digit_five(prod)) {
                    if (seq_len == seq_cap) {
                        seq_cap = seq_cap ? seq_cap * 2 : 64;
                        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    }
                    seq[seq_len++] = cand;
                    mark_used(cand);
                    break;
                }
            }
            cand++;
        }
    }

    return seq[n - 1];
}
