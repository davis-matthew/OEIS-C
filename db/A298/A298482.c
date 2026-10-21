/*
 * A298482  a(n) = smallest integer not yet in the sequence with no digits in
 *          common with a(n-1), a(n-2), and a(n-3); a(0)=0, a(1)=1, a(2)=2.
 * https://oeis.org/A298482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the definition:
 * a(0)=0, a(1)=1, a(2)=2.
 * For n≥3, a(n) is the smallest integer not yet in the
 * sequence that shares no decimal digit with a(n‑1), a(n‑2) or a(n‑3).
 * The function keeps a memoised cache of previously computed values
 * and a bitset of used integers, both dynamically grown.
 * It works for any seq_t that supports the operators +, -, /, %,
 * comparisons and increment. It also works when seq_t is a big
 * arbitrary‑precision class; the casted reallocs keep the code valid
 * in both C (implicit void* conversion) and C++ (explicit cast). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static seq_t *seq_cache = NULL;              /* stored sequence values */
static size_t cache_len = 0;                 /* number of values currently stored */

static uint64_t *used_bits = NULL;           /* bitset for used integers */
static size_t used_words = 0;

/* Build a bit mask of the decimal digits present in x. */
static unsigned int digit_mask(seq_t x)
{
    unsigned int mask = 0;
    if (x == (seq_t)0)
        return 1;          /* only digit 0 */
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* Check whether x has already been used. */
static bool used_value(seq_t x)
{
    idx_t idx = (idx_t)x;
    if ((idx >> 6) >= used_words)
        return false;
    return ((used_bits[idx >> 6] >> (idx & 63)) & 1u) != 0;
}

/* Mark x as used, expanding the bitset if necessary. */
static void set_used_value(seq_t x)
{
    idx_t idx = (idx_t)x;
    size_t w = (size_t)(idx >> 6);
    if (w >= used_words) {
        size_t new_words = used_words ? used_words * 2 : 16;
        if (new_words < w + 1)
            new_words = w + 1;
        uint64_t *newb = (uint64_t*)realloc(used_bits,
                                            new_words * sizeof(uint64_t));
        if (!newb) exit(EXIT_FAILURE);   /* allocation failed */
        /* zero the new words */
        memset(newb + used_words, 0,
               (new_words - used_words) * sizeof(uint64_t));
        used_bits = newb;
        used_words = new_words;
    }
    used_bits[w] |= (uint64_t)1ULL << (idx & 63);
}

/* Ensure that seq_cache contains terms up to index upto (inclusive). */
static void ensure_cache(idx_t upto)
{
    if (upto < cache_len)
        return;

    /* Resize the cache array. */
    size_t newlen = (size_t)upto + 1;
    seq_t *newc = (seq_t*)realloc(seq_cache, newlen * sizeof(seq_t));
    if (!newc && newlen > 0) exit(EXIT_FAILURE); /* allocation failed */
    seq_cache = newc;

    /* Compute missing terms. */
    for (idx_t i = (idx_t)cache_len; i <= upto; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)0;
        else if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)2;
        else {
            unsigned int mask = 0;
            mask |= digit_mask(seq_cache[i-1]);
            mask |= digit_mask(seq_cache[i-2]);
            mask |= digit_mask(seq_cache[i-3]);

            seq_t cand = (seq_t)0;
            for (;;) {
                if (!used_value(cand)) {
                    unsigned int cm = digit_mask(cand);
                    if ((cm & mask) == 0) {  /* digit‑disjoint */
                        val = cand;
                        break;
                    }
                }
                ++cand;
            }
        }
        seq_cache[i] = val;
        set_used_value(val);
    }
    cache_len = newlen;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
