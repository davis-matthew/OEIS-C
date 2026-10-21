/*
 * A256062  a(1) = 1; for n>1, a(n) = smallest number > a(n-1) such that the
 *          pairwise differences of elements are distinct, and for 1<m<n, a(m)
 *          does not divide a(n).
 * https://oeis.org/A256062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: greedy construction of A256062 derived by iterating candidates > previous
 * term, checking that all pairwise differences are distinct and that no earlier term
 * divides the new one. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq = NULL;        /* stored sequence values */
static size_t seq_len = 0;       /* number of stored terms */
static size_t seq_cap = 0;       /* allocated capacity */

static uint64_t *diff_bits = NULL;   /* bitset of used differences */
static size_t diff_words = 0;        /* number of 64‑bit words allocated */

#define MAX_N_PC 20000   /* safety limit for in‑stack change tracking */

typedef struct { uint64_t word; uint64_t mask; } ChangedBit;
static ChangedBit changed[MAX_N_PC];
static size_t changed_cnt = 0;

/* --- helpers for dynamic arrays ------------------------------------- */
static void ensure_seq_cap(size_t cap)
{
    if (cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    if (new_cap < cap) new_cap = cap;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq)
    {
        memcpy(new_seq, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = new_seq;
    seq_cap = new_cap;
}

static void ensure_diff_cap(uint64_t word)
{
    if (word < diff_words) return;
    size_t new_words = diff_words ? diff_words << 1 : 1024;
    while (new_words <= word) new_words <<= 1;
    uint64_t *new_bits = (uint64_t *)calloc(new_words, sizeof(uint64_t));
    if (diff_bits)
    {
        memcpy(new_bits, diff_bits, diff_words * sizeof(uint64_t));
        free(diff_bits);
    }
    diff_bits = new_bits;
    diff_words = new_words;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined but safe fallback */

    /* initialise with first term if necessary */
    if (seq_len == 0)
    {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* generate terms up to n */
    while (seq_len < n)
    {
        seq_t last = seq[seq_len - 1];
        seq_t cand = last + (seq_t)1;

        for (;;)
        {
            bool ok = true;

            /* condition: no earlier term (except 1) divides the candidate */
            for (size_t i = 1; i < seq_len; ++i)
            {
                if (cand % seq[i] == 0)
                {
                    ok = false;
                    break;
                }
            }
            if (!ok)
            {
                cand = cand + (seq_t)1;
                continue;
            }

            /* condition: all differences with earlier terms must be distinct
               and not equal to any previous difference */
            changed_cnt = 0;
            for (size_t i = 0; i < seq_len; ++i)
            {
                seq_t d = cand - seq[i];
                uint64_t du = (uint64_t)d;          /* we assume diff fits in 64‑bit */
                uint64_t word = du >> 6;
                uint64_t mask = (uint64_t)1 << (du & 63);
                ensure_diff_cap(word);
                if (diff_bits[word] & mask)
                {
                    ok = false;
                    break;
                }
                diff_bits[word] |= mask;
                changed[changed_cnt].word = word;
                changed[changed_cnt].mask = mask;
                ++changed_cnt;
            }

            if (!ok)
            {
                /* revert any bits set during this trial */
                for (size_t j = 0; j < changed_cnt; ++j)
                {
                    uint64_t w = changed[j].word;
                    diff_bits[w] ^= changed[j].mask;
                }
                cand = cand + (seq_t)1;
                continue;
            }

            /* candidate accepted */
            break;
        }

        /* store accepted candidate */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = cand;
        ++seq_len;
    }

    return seq[n - 1];
}
