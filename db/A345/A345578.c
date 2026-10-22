/*
 * A345578  Numbers that are the sum of eight fourth powers in three or more
 *          ways.
 * https://oeis.org/A345578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Figures the elements of OEIS A345578.
 *
 * The sequence consists of numbers that can be written as a sum of eight
 * fourth powers in at least three distinct unordered ways.
 * This implementation enumerates all unordered 8‑tuples of base
 * numbers (with repetitions allowed) up to a growing bound,
 * counts how many times each sum occurs, and collects the numbers
 * that reach a count of 3 or more.
 *
 * The algorithm uses a fixed base (initially 20) that turns out to
 * produce more than 10 000 terms.  If the requested index is beyond
 * the pre‑computed range, the base is doubled and the sequence is
 * recomputed from scratch.  The entire sequence is cached in a static
 * array so subsequent calls are O(1).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the requested function */
seq_t a(idx_t n);

/* Static cache */
static seq_t *cached_seq = NULL;     /* Array of discovered terms */
static size_t cached_len  = 0;        /* Length of cached_seq   */
static idx_t  current_base = 20;      /* Current base for enumeration */
static bool   initialized   = false;  /* Flag for one‑time initialization */

/* ------------------------------------------------------------------ */
/* Build the whole sequence using a given base value (>=1).           */
/* The function frees any previously cached sequence.                 */
/* ------------------------------------------------------------------ */
static void build_sequence(idx_t base)
{
    /* Pre‑compute powers of fourth powers up to 'base' */
    seq_t *pow4 = (seq_t *)malloc((size_t)(base + 1) * sizeof(seq_t));
    pow4[0] = 0;                         /* dummy entry */
    for (idx_t i = 1; i <= base; ++i) {
        seq_t val = (seq_t)i * i * i * i; /* i^4, promoted to seq_t */
        pow4[i] = val;
    }

    /* Total maximum sum: 8 * (base)^4 */
    seq_t max_sum_seq = (seq_t)8 * pow4[base];
    size_t max_sum = (size_t)max_sum_seq;

    /* Allocate counter array: counts up to 65535 are sufficient          */
    uint16_t *cnt = (uint16_t *)calloc(max_sum + 1, sizeof(uint16_t));
    if (!cnt) {
        free(pow4);
        return;                   /* out of memory – caller will try again   */
    }

    /* Enumerate all unordered 8‑tuples (nondecreasing indices) and
       count the resulting sums. */
    for (idx_t a = 1; a <= base; ++a) {
        seq_t sum_a = pow4[a];
        for (idx_t b = a; b <= base; ++b) {
            seq_t sum_ab = sum_a + pow4[b];
            for (idx_t c = b; c <= base; ++c) {
                seq_t sum_abc = sum_ab + pow4[c];
                for (idx_t d = c; d <= base; ++d) {
                    seq_t sum_abcd = sum_abc + pow4[d];
                    for (idx_t e = d; e <= base; ++e) {
                        seq_t sum_abcde = sum_abcd + pow4[e];
                        for (idx_t f = e; f <= base; ++f) {
                            seq_t sum_abcdef = sum_abcde + pow4[f];
                            for (idx_t g = f; g <= base; ++g) {
                                seq_t sum_abcdefg = sum_abcdef + pow4[g];
                                for (idx_t h = g; h <= base; ++h) {
                                    seq_t total = sum_abcdefg + pow4[h];
                                    size_t idx = (size_t)total;   /* <= max_sum */
                                    cnt[idx]++;                  /* safe: uint16_t */
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* First pass: count how many values satisfy the threshold. */
    size_t needed = 0;
    for (size_t s = 1; s <= max_sum; ++s) {
        if (cnt[s] >= 3) ++needed;
    }

    /* Allocate the new sequence buffer */
    seq_t *new_seq = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!new_seq) {
        free(cnt);
        free(pow4);
        return;
    }

    /* Second pass: store the qualifying values in ascending order. */
    size_t pos = 0;
    for (size_t s = 1; s <= max_sum; ++s) {
        if (cnt[s] >= 3) {
            new_seq[pos++] = (seq_t)s;
        }
    }

    /* Replace the old cached sequence (freeing its memory) */
    free(cached_seq);
    cached_seq = new_seq;
    cached_len = needed;

    /* Clean up temporary structures */
    free(cnt);
    free(pow4);
}

/* ------------------------------------------------------------------ */
/* Ensure that the cached sequence contains at least 'n' terms.        */
/* If not, rebuild with an increased bound.                            */
/* ------------------------------------------------------------------ */
static void ensure_sequence(idx_t n)
{
    if (!initialized) {
        initialized = true;
        build_sequence(current_base);
    }

    /* Rebuild with larger base if required */
    while (cached_len < n) {
        current_base *= 2;          /* increase search space */
        build_sequence(current_base);
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n = 0 */
    ensure_sequence(n);
    return cached_seq[n - 1];
}
