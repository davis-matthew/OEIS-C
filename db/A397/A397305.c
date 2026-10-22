/*
 * A397305  Positive numbers m such that m - 2^k is composite for all k > 0
 *          with 2^k < m.
 * https://oeis.org/A397305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A397305: brute‑force search for m such that m - 2^k is composite
   for all k > 0 with 2^k < m.  Sequence is generated lazily and
   cached for future calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

static seq_t *seq      = NULL;   /* stored terms */
static size_t seq_len  = 0;      /* number of terms stored   */
static size_t seq_cap  = 0;      /* current array capacity   */
static uint64_t next_candidate = 8; /* smallest m to test */

/* Return true iff m satisfies the definition of the sequence. */
static inline bool passes(seq_t m)
{
    seq_t pow2 = (seq_t)2; /* start with 2^1 */
    while (pow2 < m) {
        seq_t diff = m - pow2;
        if (diff <= (seq_t)1 || isPrimeTrial(diff))
            return false;
        pow2 <<= 1; /* next power of two */
    }
    return true;
}

/* Ensure there is room for at least one more term in the cached array. */
static void ensure_capacity(void)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

/* Return the nth term of A397305 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        if (passes(cand)) {
            ensure_capacity();
            seq[seq_len++] = cand;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
