/*
 * A375110  Lexicographically earliest sequence of positive integers such that
 *          the pairs (a(n) mod a(n+1), a(n+1)) are all distinct.
 * https://oeis.org/A375110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375110 – lexicographically earliest sequence of positive integers
 *  such that all pairs (a(n) mod a(n+1), a(n+1)) are distinct.
 *
 *  The algorithm constructs the sequence incrementally.  For each
 *  candidate next value m it maintains a bit‑set of the residues r
 *  that have already been used with that m.  For a[n] we try m = 1, 2,
 *  … until we find an r that has not yet appeared with that m; that
 *  m becomes a[n+1].  The bit‑set per m is created lazily and grows
 *  as needed.  The approach is O(n) on average and satisfies the
 *  lexicographic requirement.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Dynamic array of bit‑sets (one per possible next value m)         */
/* ------------------------------------------------------------------ */
static uint64_t **mod_masks   = NULL;   /* array of pointers to bit‑sets   */
static size_t   *mask_words   = NULL;   /* number of 64‑bit words per bit‑set */
static size_t      mask_cap   = 0;      /* capacity of the above arrays     */

/* ------------------------------------------------------------------ */
/*  Dynamic array of sequence terms a(1), a(2), …                    */
/* ------------------------------------------------------------------ */
static seq_t *seq    = NULL;   /* storage for the sequence */
static size_t  seq_cap = 0;     /* capacity of seq          */
static size_t  seq_len = 0;     /* number of terms stored   */

/* ------------------------------------------------------------------ */
/*  Ensure the mask array can reference indices up to `need-1`.      */
/* ------------------------------------------------------------------ */
static void
ensure_masks_cap(size_t need)
{
    if (need <= mask_cap) return;

    size_t new_cap = mask_cap ? mask_cap : 16;
    if (new_cap < need) new_cap = need;
    while (new_cap < need) new_cap <<= 1;

    uint64_t **new_ptr = (uint64_t **)realloc(mod_masks,
                                              new_cap * sizeof(uint64_t *));
    if (!new_ptr) abort();
    memset(new_ptr + mask_cap, 0,
           (new_cap - mask_cap) * sizeof(uint64_t *));
    mod_masks = new_ptr;

    size_t *new_words = (size_t *)realloc(mask_words,
                                          new_cap * sizeof(size_t));
    if (!new_words) abort();
    memset(new_words + mask_cap, 0,
           (new_cap - mask_cap) * sizeof(size_t));
    mask_words = new_words;

    mask_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Ensure the sequence array has capacity at least `req`.            */
/* ------------------------------------------------------------------ */
static void
ensure_seq_cap(size_t req)
{
    if (req <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap : 16;
    if (new_cap < req) new_cap = req;
    while (new_cap < req) new_cap <<= 1;

    seq_t *new_seq = (seq_t *)realloc(seq,
                                      new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  External linkage as required.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* behaviour for indices < 1 is irrelevant */

    /* Lazily generate terms up to index n. */
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {          /* initialise a(1) = 1 */
            ensure_seq_cap(1);
            seq[0] = (seq_t)1;
            seq_len = 1;
            continue;
        }

        seq_t current = seq[seq_len - 1];

        /* Search for the smallest possible next value `m`. */
        for (uint64_t cand = 1; ; ++cand) {
            seq_t cand_t = (seq_t)cand;

            /* Modulus r = current % cand_t. */
            seq_t r_t = current % cand_t;
            size_t r = (size_t)r_t;   /* r < cand, fits in size_t */

            /* Ensure the mask table can reference this `cand`. */
            if (cand >= mask_cap)
                ensure_masks_cap(cand + 1);

            /* Lazily allocate the bit‑set for this `cand`. */
            if (!mod_masks[cand]) {
                size_t words = (cand + 63u) >> 6;
                uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
                if (!bits) abort();
                mod_masks[cand]   = bits;
                mask_words[cand] = words;
            }

            /* Check if the pair (r, cand) has already been used. */
            size_t word_idx = r >> 6;
            uint64_t bit = 1ULL << (r & 63u);

            if ((mod_masks[cand][word_idx] & bit) == 0) {
                /* New pair – record it and append to the sequence. */
                mod_masks[cand][word_idx] |= bit;

                ensure_seq_cap(seq_len + 1);
                seq[seq_len] = cand_t;
                ++seq_len;
                break; /* move to next position in the sequence */
            }
            /* otherwise try the next candidate value */
        }
    }

    return seq[n - 1];
}
