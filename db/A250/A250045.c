/*
 * A250045  Numbers n such that m = floor(n/8) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 2261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250045: numbers n such that m = floor(n/8) is not coprime to n
 * and, if m>0, m is also a term of the sequence. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;         /* array of sequence values   */
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint8_t *isseq = NULL;     /* membership flag array      */
static size_t isseq_cap = 0;
static idx_t next_candidate = 8;

/* Ensure that 'seq' has room for at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 128;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = newcap;
}

/* Ensure that 'isseq' can index up to at least 'needed - 1'. */
static void ensure_isseq_capacity(size_t needed)
{
    if (needed <= isseq_cap) return;
    size_t newcap = isseq_cap ? isseq_cap << 1 : 256;
    while (newcap < needed) newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(isseq, newcap * sizeof(uint8_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the newly allocated part */
    for (size_t i = isseq_cap; i < newcap; ++i) tmp[i] = 0;
    isseq = tmp;
    isseq_cap = newcap;
}

/* Initialise the sequence with the base values 2..7. */
static void init_sequence(void)
{
    if (seq != NULL) return;                /* already initialised */
    ensure_seq_capacity(128);
    ensure_isseq_capacity(256);

    /* Base terms 2,3,4,5,6,7 */
    for (idx_t v = 2; v <= 7; ++v) {
        seq[seq_len++] = (seq_t)v;
        if ((size_t)v >= isseq_cap) ensure_isseq_capacity((size_t)v + 1);
        isseq[v] = 1;
    }
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, safe return */
    init_sequence();

    while (seq_len < n) {
        idx_t cand = next_candidate++;
        idx_t m = cand >> 3;                /* floor(cand / 8) */

        /* Ensure 'isseq' can index m. */
        if ((size_t)m >= isseq_cap) ensure_isseq_capacity((size_t)m + 1);

        /* Check the defining property. */
        seq_t g = gcd((seq_t)cand, (seq_t)m);
        if (g > (seq_t)1) {
            if (m == 0 || isseq[m]) {
                /* New term */
                if ((size_t)cand >= isseq_cap) ensure_isseq_capacity((size_t)cand + 1);
                isseq[cand] = 1;
                if (seq_len + 1 > seq_cap) ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
            }
        }
    }
    return seq[n - 1];
}
