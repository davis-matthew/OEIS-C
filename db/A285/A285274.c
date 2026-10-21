/*
 * A285274  0-limiting word of the morphism 0->10, 1-> 0111.
 * https://oeis.org/A285274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285274: nth term of the limit word of the morphism 0->10, 1->0111
   (the even‑iteration limiting word). */

#include <stdlib.h>
#include <stdint.h>

/* Helper: compute the symbol (0 or 1) at position `pos` (1‑based) in the word
   obtained after `depth` iterations of the morphism, starting from the root
   symbol `sym`. */
static seq_t get_symbol(idx_t sym, size_t depth, idx_t pos,
                       const idx_t *len0, const idx_t *len1)
{
    while (depth > 0)
    {
        if (sym == 0)
        {
            idx_t part1 = len1[depth - 1]; /* expansion of `1` */
            idx_t part0 = len0[depth - 1]; /* expansion of `0` */
            if (pos <= part1)
            {
                sym = 1; /* go to the first child */
            }
            else
            {
                pos -= part1;
                sym = 0; /* go to the second child */
            }
        }
        else /* sym == 1 */
        {
            idx_t c0 = len0[depth - 1];
            idx_t c1 = len1[depth - 1];
            /* children: 0,1,1,1 */
            if (pos <= c0)
            {
                sym = 0;
            }
            else if (pos <= c0 + c1)
            {
                pos -= c0;
                sym = 1;
            }
            else if (pos <= c0 + c1 + c1)
            {
                pos -= c0 + c1;
                sym = 1;
            }
            else
            {
                pos -= c0 + 2 * c1;
                sym = 1;
            }
        }
        depth--;
    }
    return (seq_t)sym;
}

seq_t a(idx_t n)
{
    /* The target position (1‑based) */
    idx_t target = n;

    /* Dynamically grow arrays that hold lengths of the expansions of
       symbols 0 and 1 after `d` iterations.  We keep the values capped
       at `target` because larger lengths do not affect the decision
       for positions <= target. */
    size_t depth = 0;
    idx_t *len0 = (idx_t *)malloc(sizeof(idx_t));
    idx_t *len1 = (idx_t *)malloc(sizeof(idx_t));
    if (!len0 || !len1)
        return (seq_t)0; /* allocation failure; not expected */

    len0[0] = 1;
    len1[0] = 1;

    /* Grow until the length of the 0‑word after `depth` iterations
       covers the target position. */
    while (len0[depth] < target)
    {
        depth++;
        len0 = (idx_t *)realloc(len0, sizeof(idx_t) * (depth + 1));
        len1 = (idx_t *)realloc(len1, sizeof(idx_t) * (depth + 1));
        if (!len0 || !len1)
            return (seq_t)0; /* allocation failure */

        idx_t prev0 = len0[depth - 1];
        idx_t prev1 = len1[depth - 1];

        /* Compute next lengths, capping at target to avoid overflow. */
        __uint128_t t0 = (__uint128_t)prev0 + (__uint128_t)prev1;
        __uint128_t t1 = (__uint128_t)prev0 + (__uint128_t)3 * (__uint128_t)prev1;
        len0[depth] = (t0 > target) ? target : (idx_t)t0;
        len1[depth] = (t1 > target) ? target : (idx_t)t1;
    }

    /* Ensure the depth is even (even‑iteration limiting word). */
    if (depth & 1U)
    {
        depth++;
        len0 = (idx_t *)realloc(len0, sizeof(idx_t) * (depth + 1));
        len1 = (idx_t *)realloc(len1, sizeof(idx_t) * (depth + 1));
        if (!len0 || !len1)
            return (seq_t)0; /* allocation failure */

        idx_t prev0 = len0[depth - 1];
        idx_t prev1 = len1[depth - 1];

        __uint128_t t0 = (__uint128_t)prev0 + (__uint128_t)prev1;
        __uint128_t t1 = (__uint128_t)prev0 + (__uint128_t)3 * (__uint128_t)prev1;
        len0[depth] = (t0 > target) ? target : (idx_t)t0;
        len1[depth] = (t1 > target) ? target : (idx_t)t1;
    }

    /* Compute the symbol at position `n`.  The root symbol is 0. */
    seq_t res = get_symbol(0, depth, target, len0, len1);

    free(len0);
    free(len1);
    return res;
}
