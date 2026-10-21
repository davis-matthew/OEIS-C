/*
 * A285668  Fixed point of the morphism 0->11, 1->1101.
 * https://oeis.org/A285668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 930, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A285668: a(n) is the n‑th symbol of the fixed point of the
 * morphism 0 → 11, 1 → 1101.
 * The word starts with 1, so a(1) = 1.  The sequence is 1,1,0,1,1,1,0,1,...
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* lengths of f^k(0) and f^k(1).  The base case (k = 0) has length 1. */
    static seq_t len0[64];
    static seq_t len1[64];

    static size_t last = 0;   /* highest k with known lengths */
    static int    init  = 0;  /* one‑time initialization flag */

    if (!init) {
        len0[0] = (seq_t)1;   /* |f^0(0)| = |f^0(1)| = 1 */
        len1[0] = (seq_t)1;
        last    = 0;
        init    = 1;
    }

    /* Grow tables until len1[last] ≥ n.  The fixed point starts with 1. */
    while (len1[last] < (seq_t)n) {
        size_t k = last + 1;
        len0[k] = (seq_t)2 * len1[last];          /* f(0): two 1’s */
        len1[k] = (seq_t)3 * len1[last] + len0[last]; /* f(1): 1,1,0,1 */
        last = k;
    }

    size_t level = last;        /* depth of recursion */
    int val = 1;                /* root symbol is 1 */

    while (level > 0) {
        if (val == 0) {
            /* 0 → 1 1 : two copies of f^{level‑1}(1) */
            seq_t child_len = len1[level - 1];
            if ((seq_t)n <= child_len) {
                val = 1;       /* first child */
            } else {
                n -= (idx_t)child_len;  /* second child */
                val = 1;
            }
            level--;
        } else {
            /* 1 → 1 1 0 1 */
            seq_t l1 = len1[level - 1];
            seq_t l0 = len0[level - 1];
            if ((seq_t)n <= l1) {                      /* first child */
                val = 1;
            } else if ((seq_t)n <= l1 + l1) {          /* second child */
                n -= (idx_t)l1;
                val = 1;
            } else if ((seq_t)n <= l1 + l1 + l0) {     /* third child (0) */
                n -= (idx_t)(l1 + l1);
                val = 0;
            } else {                                   /* fourth child */
                n -= (idx_t)(l1 + l1 + l0);
                val = 1;
            }
            level--;
        }
    }

    return (seq_t)val;
}
