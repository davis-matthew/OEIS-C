/*
 * A259429  With a(1) = 1, a(n) is the smallest number not already in the
 *          sequence such that the arithmetic mean of two neighboring terms is
 *          a cube.
 * https://oeis.org/A259429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=1.  For n>1, a(n) is the smallest unused odd integer
   such that (a(n-1)+a(n))/2 is a perfect cube.  The sequence is a
   permutation of the odd integers.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/big_pow.h"

static seq_t *seq            = NULL;   /* sequence, 0‑based */
static size_t seq_len       = 0;       /* number of terms stored   */
static size_t seq_cap       = 0;       /* allocated capacity       */
static uint8_t *used_bits    = NULL;   /* bitset for used odd ints */
static size_t used_bytes     = 0;       /* bytes allocated for used_bits */
static seq_t max_used_val    = 0;       /* largest value that has ever been used */

/* Ensure used_bits can index up to `max_val`.  */
static void ensure_used_capacity(seq_t max_val)
{
    size_t bit_index = (size_t)(max_val >> 1);
    size_t bits_needed = bit_index + 1;
    size_t bytes_needed = (bits_needed + 7) >> 3;
    if (bytes_needed > used_bytes) {
        uint8_t *new_ptr = (uint8_t *)realloc(used_bits, bytes_needed);
        if (!new_ptr) abort();                    /* allocation failure */
        memset(new_ptr + used_bytes, 0, bytes_needed - used_bytes);
        used_bits  = new_ptr;
        used_bytes = bytes_needed;
    }
}

/* Mark the odd integer `val` as used.  */
static void set_used(seq_t val)
{
    size_t bit_index = (size_t)(val >> 1);
    used_bits[bit_index >> 3] |= (uint8_t)(1 << (bit_index & 7));
}

/* Test whether the odd integer `val` has already been used.  */
static bool is_used(seq_t val)
{
    size_t bit_index = (size_t)(val >> 1);
    return (used_bits[bit_index >> 3] >> (bit_index & 7)) & 1;
}

/* Append a new value to the sequence, expanding the array if needed. */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) abort();          /* allocation failure */
        seq      = new_seq;
        seq_cap  = new_cap;
    }
    seq[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of range; offset is 1 */

    while (seq_len < n) {
        if (seq_len == 0) {                /* first term */
            seq_push((seq_t)1);
            ensure_used_capacity((seq_t)1);
            set_used((seq_t)1);
            max_used_val = 1;
            continue;
        }

        seq_t prev   = seq[seq_len - 1];
        seq_t chosen = 0;

        for (idx_t k = 1; ; ++k) {
            seq_t cube  = big_pow((seq_t)k, (seq_t)3); /* k^3 */
            seq_t twice = cube << 1;                   /* 2*k^3 */
            if (twice <= prev) continue;              /* would give <=0 */
            seq_t cand = twice - prev;                 /* candidate */
            if ((cand > max_used_val) || !is_used(cand)) {
                chosen = cand;
                break;
            }
        }

        seq_push(chosen);
        ensure_used_capacity(chosen);
        set_used(chosen);
        if (chosen > max_used_val) max_used_val = chosen;
    }

    return seq[n - 1];
}
