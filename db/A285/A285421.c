/*
 * A285421  1-limiting word of the morphism 0->11, 1-> 011.
 * https://oeis.org/A285421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth letter of the infinite word obtained by
 *            iterating the morphism 0→11, 1→011, starting from a single 0 and
 *            taking only odd iterations. The word is built recursively; this
 *            function returns the terms as listed in OEIS A285421
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *len0 = NULL; /* length of word starting with 0 after d iterations */
static idx_t *len1 = NULL; /* length of word starting with 1 after d iterations */
static size_t cap = 0;     /* allocated size of the len arrays */
static idx_t depth_cap = 0;/* highest iteration computed (always odd after ensure_depth) */

/* Grow length tables until len0[depth_cap] ≥ n and depth_cap is odd. */
static void ensure_depth(idx_t n)
{
    if (!len0) {
        /* initial allocation */
        cap = 8;
        len0 = (idx_t *)malloc(cap * sizeof(idx_t));
        len1 = (idx_t *)malloc(cap * sizeof(idx_t));
        /* base case: a single symbol has length 1 */
        len0[0] = len1[0] = (idx_t)1;
        depth_cap = 0;
    }

    /* extend until we have enough odd depth */
    while (len0[depth_cap] < n || (depth_cap & 1) == 0) {
        /* resize if necessary */
        if (depth_cap + 1 >= cap) {
            size_t new_cap = (cap == 0) ? 8 : cap << 1;
            len0 = (idx_t *)realloc(len0, new_cap * sizeof(idx_t));
            len1 = (idx_t *)realloc(len1, new_cap * sizeof(idx_t));
            cap = new_cap;
        }

        idx_t d = depth_cap + 1;

        /* compute lengths for depth d using values from depth d-1 */
        __int128 t0 = (__int128)len1[depth_cap] * 2;                       /* len0[d] = 2*len1[d-1] */
        len0[d] = (t0 > (__int128)(idx_t)-1) ? (idx_t)-1 : (idx_t)t0;

        __int128 t1 = (__int128)len0[depth_cap] + (__int128)len1[depth_cap] * 2; /* len1[d] = len0[d-1] + 2*len1[d-1] */
        len1[d] = (t1 > (__int128)(idx_t)-1) ? (idx_t)-1 : (idx_t)t1;

        depth_cap = d;
    }
}

/* Recursively find the symbol at position pos (1‑based) after 'depth'
   iterations, starting from symbol 'sym'. */
static seq_t get_bit(seq_t sym, idx_t depth, idx_t pos)
{
    if (depth == 0) {
        return sym;
    }

    if (sym == 0) {       /* 0 → 1 1 */
        idx_t child_len = len1[depth - 1];
        if (pos <= child_len) {
            return get_bit((seq_t)1, depth - 1, pos);
        }
        pos -= child_len; /* second child */
        return get_bit((seq_t)1, depth - 1, pos);
    } else {              /* 1 → 0 1 1 */
        idx_t len0_child = len0[depth - 1];
        if (pos <= len0_child) {
            return get_bit((seq_t)0, depth - 1, pos);
        }
        pos -= len0_child;

        idx_t len1_child = len1[depth - 1];
        if (pos <= len1_child) {
            return get_bit((seq_t)1, depth - 1, pos);
        }
        pos -= len1_child; /* third child */
        return get_bit((seq_t)1, depth - 1, pos);
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Ensure the length tables cover the required odd iteration. */
    ensure_depth(n);
    /* The limit word is built from a single 0 after an odd number of iterations. */
    return get_bit((seq_t)0, depth_cap, n);
}
