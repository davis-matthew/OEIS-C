/*
 * A368546  Alternative version of the Markov tree A327345.
 * https://oeis.org/A368546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 39, u64 n <= 104, u128 n <= 328,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A368546 – Middle element of the nth node (BFS order) of the Markov tree.
   The tree starts from triple (1,5,2).
   Left child of (a,b,c) is (a, 3*a*b - c, b).
   Right child of (a,b,c) is (b, 3*b*c - a, c).  */

/* This routine memoises all needed triples so that any n is answered in amortised
   O(1) time after a one‑time linear‑ish build.  The sequence is finally
   returned from the pre‑computed array `middles`. */

#include <stdlib.h>
#include <stdint.h>   /* for NULL */

struct Triple
{
    seq_t a;
    seq_t b;
    seq_t c;
};

/* dynamic arrays of triples and middle elements */
static struct Triple *triples = NULL;
static size_t triples_len   = 0;
static size_t triples_cap   = 0;

static seq_t *middles = NULL;
static size_t middles_len = 0;
static size_t middles_cap = 0;

/* index of the next triple to process in BFS order */
static size_t processed = 0;

/* ensure capacity for triples array */
static void ensure_triples_cap(size_t needed)
{
    if (needed > triples_cap)
    {
        size_t new_cap = triples_cap ? triples_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        struct Triple *tmp = (struct Triple *)realloc(triples, new_cap * sizeof(struct Triple));
        triples = tmp;
        triples_cap = new_cap;
    }
}

/* ensure capacity for middles array */
static void ensure_middles_cap(size_t needed)
{
    if (needed > middles_cap)
    {
        size_t new_cap = middles_cap ? middles_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(middles, new_cap * sizeof(seq_t));
        middles = tmp;
        middles_cap = new_cap;
    }
}

/* append a new triple and its middle to the arrays */
static void append_triple(seq_t a, seq_t b, seq_t c)
{
    ensure_triples_cap(triples_len + 1);
    triples[triples_len].a = a;
    triples[triples_len].b = b;
    triples[triples_len].c = c;
    triples_len++;

    ensure_middles_cap(middles_len + 1);
    middles[middles_len] = b;
    middles_len++;
}

/* initialise root of the tree if not yet done */
static void init_root(void)
{
    if (triples_len == 0)
    {
        append_triple((seq_t)1, (seq_t)5, (seq_t)2);
        /* root processed count starts at 0 */
    }
}

seq_t a(idx_t n)
{
    /* ensure we have computed enough terms */
    if (triples_len == 0) init_root();

    /* build sequence until we have the required index (0‑based) */
    while (middles_len <= (size_t)n)
    {
        /* process next unprocessed node to generate its two children */
        struct Triple *cur = &triples[processed];
        seq_t a_val = cur->a;
        seq_t b_val = cur->b;
        seq_t c_val = cur->c;

        /* left child */
        append_triple(a_val, (seq_t)3 * a_val * b_val - c_val, b_val);
        /* right child */
        append_triple(b_val, (seq_t)3 * b_val * c_val - a_val, c_val);

        processed++;
    }

    /* return requested value */
    return middles[(size_t)n];
}
