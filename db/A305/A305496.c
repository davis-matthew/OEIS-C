/*
 * A305496  Positions of 2 in the fixed point of the morphism 0->120, 1->110,
 *          2->100 applied to 1 (as in A305490).
 * https://oeis.org/A305496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305496 – Positions of 2 in the fixed point of
   the morphism 0→120, 1→110, 2→100 starting from 1.
   The symbol at position n is obtained by iterating the
   morphism on the base symbol 1; the positions of symbol 2
   are stored in a dynamic array.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Substitution table: 0→[1,2,0], 1→[1,1,0], 2→[1,0,0] */
static const unsigned char sub[3][3] = {
    {1,2,0},   /* 0 → 1 2 0 */
    {1,1,0},   /* 1 → 1 1 0 */
    {1,0,0}    /* 2 → 1 0 0 */
};

/* Apply the morphism iteratively to obtain the symbol at
   position pos (1‑based).  The argument is 1‑based. */
static unsigned char
letter_at(idx_t pos)
{
    unsigned char c = 1;          /* the starting symbol 1 */
    pos--;                        /* work with a zero‑based value */
    /* Convert pos to base‑3 digits (least significant first). */
    idx_t v = pos;
    int digits[64], d = 0;
    while (v) {
        digits[d++] = v % 3;
        v /= 3;
    }
    /* Apply the morphism in reverse order (most significant first). */
    for (int i = d - 1; i >= 0; --i)
        c = sub[c][digits[i]];
    return c;
}

/* Dynamic array of positions where the symbol is 2 */
static seq_t *pos2      = NULL;   /* array */
static size_t pos2_len  = 0;      /* number of entries stored */
static size_t pos2_cap  = 0;      /* current capacity */
static idx_t next_pos   = 1;      /* next position to examine */

/* Extend the cache until at least n entries of 2 are found. */
static void
ensure_n(size_t n)
{
    if (pos2_len >= n)
        return;
    if (pos2_cap == 0) {
        pos2_cap = 128;
        pos2 = (seq_t *)malloc(pos2_cap * sizeof(seq_t));
    }
    while (pos2_len < n) {
        if (pos2_len >= pos2_cap) {
            pos2_cap *= 2;
            pos2 = (seq_t *)realloc(pos2, pos2_cap * sizeof(seq_t));
        }
        unsigned char sym = letter_at(next_pos);
        if (sym == 2)
            pos2[pos2_len++] = (seq_t)next_pos;
        ++next_pos;
    }
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    ensure_n((size_t)n);
    return pos2[n-1];
}
