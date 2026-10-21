/*
 * A244724  Lexicographically earliest permutation of the natural numbers such
 *          that primes and composites alternate in the sums of adjacent
 *          terms.
 * https://oeis.org/A244724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244724: lexicographically earliest permutation of the natural
   numbers such that the sums of adjacent terms alternate between prime
   (for odd index sums) and composite (for even index sums).  The
   implementation builds the sequence incrementally and caches all
   generated values.  Uses the generic primality test `isPrimeGeneric`
   from the utils library. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n) */
#include "../utils/seq_realloc.h"     /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* --- Global memoisation state ------------------------------------------ */
static seq_t *seq = NULL;        /* dynamically‑grown sequence array */
static size_t seq_cap = 0;       /* allocated capacity of seq */
static size_t seq_len = 0;       /* number of values stored in seq */

static uint8_t *used = NULL;     /* bitmap of numbers already used */
static size_t used_cap = 0;      /* capacity of the bitmap */

/* --- Ensure that `used` can index up to `maxIdx` (inclusive). --------- */
static void ensure_used_capacity(size_t maxIdx)
{
    if (used_cap <= maxIdx) {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        while (new_cap <= maxIdx) new_cap <<= 1;  /* grow by factors of two */

        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) abort();
        if (new_cap > used_cap)
            memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
        used = tmp;
        used_cap = new_cap;
    }
}

/* --- Ensure that `seq` can hold at least `minCap` elements. ------------ */
static void ensure_seq_capacity(size_t minCap)
{
    if (seq_cap < minCap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < minCap) new_cap <<= 1;

        seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* --- Public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    /* Generate terms up to index n (1‑based). */
    while (seq_len < (size_t)n) {
        /* Grow the sequence storage if necessary. */
        ensure_seq_capacity(seq_len + 1);

        /* First term is always 1. */
        if (seq_len == 0) {
            seq[0] = (seq_t)1;
            seq_len = 1;
            ensure_used_capacity(1);
            used[1] = 1;
            continue;
        }

        /* Determine expected primality of the next sum:
           - For odd previous index (seq_len odd) the sum must be prime.
           - For even previous index (seq_len even) the sum must be composite. */
        bool need_prime = (seq_len & 1) == 1;

        /* Search for the smallest unused integer satisfying the parity. */
        idx_t candidate = 1;
        for (;;) {
            if ((size_t)candidate >= used_cap)
                ensure_used_capacity((size_t)candidate);

            if (used[(size_t)candidate]) {
                ++candidate;
                continue;
            }

            seq_t sum = seq[seq_len - 1] + (seq_t)candidate;
            bool is_prime = isPrimeGeneric(sum);

            if (is_prime == need_prime) {
                seq[seq_len++] = (seq_t)candidate;
                used[(size_t)candidate] = 1;
                break;
            }
            ++candidate;
        }
    }

    return seq[n - 1];
}
