/*
 * A294442  Kepler's tree of fractions, read across rows (the fraction i/j is
 *          represented as the pair i,j).
 * https://oeis.org/A294442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5468, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Kepler's tree of fractions – numerators in breadth‑first order.
   a(0) = 1.
   For n > 0, a(n) is the numerator of the nth fraction of the tree
   defined by:
        root   : 1/1
        second : 1/2
        thereafter, for a fraction i/j, write the two fractions
        i/(i+j) and j/(i+j).
   The sequence consists of the numerators of those fractions in row order. */

#include <stdlib.h>

typedef struct {
    seq_t num;
    seq_t den;
} frac_t;

/* Ensure the global array has at least `needed` elements. */
static void
ensure_frac_cap(size_t needed, size_t *cap, frac_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = *cap ? *cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    frac_t *tmp = (frac_t *)realloc(*ptr, new_cap * sizeof(frac_t));
    if (!tmp)
        exit(1);               /* allocation failed */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Persistent state between calls. */
    static frac_t *nodes = NULL;   /* array of fractions */
    static size_t nodes_cap = 0;   /* allocated capacity */
    static size_t nodes_cnt = 0;   /* number of fractions stored */
    static size_t proc_idx = 0;    /* next fraction to process (BFS) */

    /* Initialise with the root node if needed. */
    if (!nodes_cnt) {
        ensure_frac_cap(1, &nodes_cap, &nodes);
        nodes[0].num = (seq_t)1;
        nodes[0].den = (seq_t)1;
        nodes_cnt = 1;
    }

    size_t target = (size_t)n;     /* assume n fits in size_t */

    /* Generate nodes until we have at least (target+1) fractions. */
    while (nodes_cnt <= target) {
        /* Current node to expand. */
        frac_t cur = nodes[proc_idx];
        seq_t i = cur.num;
        seq_t j = cur.den;
        seq_t sum = i + j;

        if (proc_idx == 0) {
            /* Root: only one child 1/2 (i/(i+j)). */
            ensure_frac_cap(nodes_cnt + 1, &nodes_cap, &nodes);
            nodes[nodes_cnt].num = i;
            nodes[nodes_cnt].den = sum;
            nodes_cnt++;
        } else {
            /* Two children. */
            ensure_frac_cap(nodes_cnt + 1, &nodes_cap, &nodes);
            nodes[nodes_cnt].num = i;
            nodes[nodes_cnt].den = sum;
            nodes_cnt++;

            ensure_frac_cap(nodes_cnt + 1, &nodes_cap, &nodes);
            nodes[nodes_cnt].num = j;
            nodes[nodes_cnt].den = sum;
            nodes_cnt++;
        }
        proc_idx++;   /* move to next node to process */
    }

    /* Return the numerator of the requested fraction. */
    return nodes[target].num;
}
