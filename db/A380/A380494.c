/*
 * A380494  a(1) = 1. For n > 1 a(n) is the smallest positive integer not yet
 *          in the sequence which is divisible by A007953(a(n-1)) + 1.
 * https://oeis.org/A380494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n) = smallest unused multiple of digit_sum(a(n-1))+1. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_sum.h"

static seq_t *seq = NULL;          /* indexed starting at 1, seq[0] unused */
static size_t seq_cap = 0;         /* maximum index currently allocated for */
static size_t seq_len = 0;         /* highest index computed so far */

static uint8_t *used = NULL;       /* bitset for numbers already in the sequence */
static size_t used_bits = 0;       /* total number of bits allocated in 'used' */

/* Ensure that 'seq' can hold indices up to 'n' (inclusive). */
static void ensure_seq_cap(size_t n)
{
    if (seq_cap >= n) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < n) new_cap *= 2;
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * (new_cap + 1));   /* +1 for index 0 */
    seq_cap = new_cap;
}

/* Ensure that 'used' can hold bits 0..(bits-1). */
static void ensure_used_bits(size_t bits)
{
    if (used_bits >= bits) return;
    size_t new_bits = used_bits ? used_bits * 2 : 8;
    while (new_bits < bits) new_bits *= 2;
    size_t old_bytes = used_bits >> 3;
    size_t new_bytes = new_bits >> 3;
    used = (uint8_t *)realloc(used, new_bytes);
    memset(used + old_bytes, 0, new_bytes - old_bytes);
    used_bits = new_bits;
}

static inline void set_used(idx_t v)
{
    size_t byte = (size_t)v >> 3;
    uint8_t mask = 1U << (v & 7);
    used[byte] |= mask;
}

static inline bool is_used(idx_t v)
{
    size_t byte = (size_t)v >> 3;
    uint8_t mask = 1U << (v & 7);
    return (used[byte] & mask) != 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value for indices < 1 is irrelevant */

    /* Initialise the first term if necessary */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = 0;
        seq[1] = 1;
        seq_len = 1;
        ensure_used_bits(2);   /* need bits 0 and 1 */
        set_used(1);
    }

    /* Compute terms up to index n if they are not yet known */
    if ((idx_t)seq_len < n) {
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t prev = seq[(size_t)(i - 1)];
            seq_t ds   = digit_sum(prev);            /* digit sum of previous term */
            seq_t divt = ds + (seq_t)1;              /* divisor to use */
            idx_t div  = (idx_t)divt;                /* divisor fits in idx_t */

            /* Find the smallest multiple of 'div' that is not yet used */
            idx_t cand = div;
            ensure_used_bits(cand + 1);              /* need bit for cand */
            while (is_used(cand)) {
                cand += div;
                ensure_used_bits(cand + 1);
            }

            seq_t cand_val = (seq_t)cand;
            ensure_seq_cap((size_t)i);
            seq[(size_t)i] = cand_val;
            set_used(cand);
        }
        seq_len = (size_t)n;
    }

    return seq[(size_t)n];
}
