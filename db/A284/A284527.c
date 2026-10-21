/*
 * A284527  1-limiting word of the morphism 0->1, 1->0010.
 * https://oeis.org/A284527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 1‑limiting word of the morphism 0->1, 1->0010.
   a(n) is the nth symbol (1‑based index) of the infinite word that
   is the limit of odd iterations of that morphism.  The word is the
   fixed point of the morphism ψ: 0→0010, 1→1100101 with seed 1. */
#include <stdint.h>

static seq_t len0[256];
static seq_t len1[256];
static idx_t max_precomp = 0;
static int precomp_done = 0;

static seq_t get_symbol(seq_t pos, int sym, idx_t level)
{
    if (level == 0) return sym;
    if (sym == 0) {
        if (pos < len0[level-1]) return get_symbol(pos, 0, level-1);
        pos -= len0[level-1];
        if (pos < len0[level-1]) return get_symbol(pos, 0, level-1);
        pos -= len0[level-1];
        if (pos < len1[level-1]) return get_symbol(pos, 1, level-1);
        pos -= len1[level-1];
        return get_symbol(pos, 0, level-1);
    } else {
        if (pos < len1[level-1]) return get_symbol(pos, 1, level-1);
        pos -= len1[level-1];
        if (pos < len1[level-1]) return get_symbol(pos, 1, level-1);
        pos -= len1[level-1];
        if (pos < len0[level-1]) return get_symbol(pos, 0, level-1);
        pos -= len0[level-1];
        if (pos < len0[level-1]) return get_symbol(pos, 0, level-1);
        pos -= len0[level-1];
        if (pos < len1[level-1]) return get_symbol(pos, 1, level-1);
        pos -= len1[level-1];
        if (pos < len0[level-1]) return get_symbol(pos, 0, level-1);
        pos -= len0[level-1];
        return get_symbol(pos, 1, level-1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – not used for valid n */
    if (!precomp_done) {
        len0[0] = 1;
        len1[0] = 1;
        max_precomp = 0;
        precomp_done = 1;
    }
    seq_t pos = (seq_t)n - 1;       /* 0‑based index within the infinite word */
    idx_t level = 0;
    while (len1[level] <= pos) {
        if (level == max_precomp) {
            idx_t next = level + 1;
            len0[next] = 3*len0[level] + len1[level];
            len1[next] = 3*len0[level] + 4*len1[level];
            max_precomp = next;
        }
        level++;
    }
    return get_symbol(pos, 1, level);
}
