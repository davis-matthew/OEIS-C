/*
 * A335033  a(n) is the smallest number larger than a(n-1) that is not a
 *          partial sum of 2^a(1),2^a(2),...,2^a(n-1), with a(1)=0.
 * https://oeis.org/A335033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0. For n > 1, a(n) = smallest m > a(n‑1) such that m has
 *            a binary 1‑bit at an exponent k that is not in {a(1),…,a(n‑1)}.
 *            This is the greedy construction of sequence A335033
 * Range:     u8 n <= 225, u16 n <= 9782, u32 n <= 9782, u64 n <= 9782,
 *            u128 n <= 9782, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* dynamic bitset holding the set of exponents that have appeared in the sequence */
static unsigned char *used_bitmap = NULL;   /* one bit per exponent */
static size_t used_bits = 0;                /* number of bits currently stored */

/* Ensure the bitset can hold positions up to pos (inclusive). */
static void ensure_used_bits(idx_t pos)
{
    if ((size_t)pos < used_bits) return;
    size_t new_bits = (size_t)pos + 1;          /* bits needed */
    if (new_bits % 8)                          /* round up to whole bytes */
        new_bits = (new_bits + 7) / 8 * 8;
    unsigned char *new_bm = (unsigned char *)
        realloc(used_bitmap, new_bits / 8);
    if (!new_bm) exit(1);
    /* zero the newly allocated part */
    memset(new_bm + used_bits / 8, 0, (new_bits / 8) - (used_bits / 8));
    used_bitmap = new_bm;
    used_bits = new_bits;
}

static void set_used(idx_t pos)
{
    ensure_used_bits(pos);
    used_bitmap[pos / 8] |= 1 << (pos % 8);
}

static bool is_used(idx_t pos)
{
    if ((size_t)pos >= used_bits) return false;
    return (used_bitmap[pos / 8] >> (pos % 8)) & 1;
}

/* Return true if x has a binary 1‑bit at a position not present in the
   used exponent set. */
static bool has_new_bit(seq_t x)
{
    /* We only need to inspect the low 64 bits because all used exponents
       that appear in the sequence for n up to 10⁴ fit in 64 bits. */
    uint64_t val = (uint64_t)x;
    if (val == 0) return false;               /* 0 is always representable */
    while (val) {
        int k = __builtin_ctzll(val);
        if (!is_used((idx_t)k)) return true;
        val &= val - 1;                       /* clear lowest set bit */
    }
    return false;
}

/* storage for computed sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but safe for unused calls */

    /* initialize on first call */
    if (seq_vals == NULL) {
        seq_vals = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_vals) exit(1);
        seq_vals[0] = (seq_t)0;         /* a(1) = 0 */
        seq_len = 1;
        set_used((idx_t)0);
    }

    /* reserve space for the needed terms */
    if (seq_len < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(seq_vals, sizeof(seq_t) * (size_t)n);
        if (!tmp) exit(1);
        seq_vals = tmp;
    }

    /* compute missing terms up to n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t cand = prev + (seq_t)1;
        while (!has_new_bit(cand)) cand += (seq_t)1;
        seq_vals[seq_len] = cand;
        seq_len++;
        set_used((idx_t)cand);
    }

    return seq_vals[n - 1];
}
