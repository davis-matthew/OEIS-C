/*
 * A316340  Image of 1 under repeated application of the morphism 1 -> 12312,
 *          2 -> 341, 3 -> 34134, 4 -> 123.
 * https://oeis.org/A316340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A316340 – the fixed point of the morphism
 *   1 → 1 2 3 1 2
 *   2 → 3 4 1
 *   3 → 3 4 1 3 4
 *   4 → 1 2 3
 * The function a(n) returns the symbol at position n of this infinite word.
 *
 * Implementation strategy:
 *   * Pre‑compute the length of the expansion of each symbol after a given
 *     number of iterations.
 *   * Find the minimal iteration depth that produces a word length
 *     exceeding n.
 *   * Recursively descend the morphism tree until reaching a leaf.
 * Since the alphabet has only four symbols, recursion depth is tiny
 * (≤64 for any n that fits in uint64_t), so the procedure is fast and
 * memory‑efficient.
 */

#include <stddef.h>   /* for NULL */
#include <stdint.h>   /* for UINT64_MAX */

/* Expansion tables – stored as unsigned char for efficient indexing. */
static const unsigned char exp1[5] = {1, 2, 3, 1, 2};
static const unsigned char exp2[3] = {3, 4, 1};
static const unsigned char exp3[5] = {3, 4, 1, 3, 4};
static const unsigned char exp4[3] = {1, 2, 3};

/* Convenient lookup: expansion_map[α] gives the expansion of symbol α (α∈{1…4}) */
static const unsigned char * const expansion_map[5] =
{
    NULL,          /* 0 is unused */
    exp1,          /* 1 → */
    exp2,          /* 2 → */
    exp3,          /* 3 → */
    exp4           /* 4 → */
};
static const unsigned int expansion_len[5] =
{
    0, 5, 3, 5, 3        /* lengths of the four expansions */
};

/* Recursive descent to obtain the symbol at position `pos`
   inside the expansion of `sym` after `depth` iterations. */
static seq_t get_sym(unsigned int sym,
                     const idx_t len[5][65],
                     int depth,
                     idx_t pos)
{
    if (depth == 0)
        return (seq_t)sym;               /* leaf: the symbol itself */

    const unsigned char *children = expansion_map[sym];
    unsigned int clen = expansion_len[sym];

    for (unsigned int i = 0; i < clen; ++i)
    {
        idx_t childlen = len[ children[i] ][depth - 1];
        if (pos < childlen)
            return get_sym((unsigned int)children[i],
                           len, depth - 1, pos);
        pos -= childlen;
    }
    /* Should never reach here; return 0 to satisfy the compiler. */
    return (seq_t)0;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Precompute word lengths up to a reasonable depth.
     * Depth 64 is more than enough for any n that fits in uint64_t. */
    idx_t len[5][65];
    for (int s = 1; s <= 4; ++s)
        len[s][0] = 1;                /* a single symbol has length 1 */

    for (int d = 1; d <= 64; ++d)
    {
        for (int s = 1; s <= 4; ++s)
        {
            idx_t sum = 0;
            const unsigned char *children = expansion_map[s];
            unsigned int clen = expansion_len[s];
            for (unsigned int i = 0; i < clen; ++i)
            {
                idx_t childlen = len[ children[i] ][d - 1];
                if (sum > UINT64_MAX - childlen)
                    sum = UINT64_MAX;   /* avoid overflow: saturate */
                else
                    sum += childlen;
            }
            len[s][d] = sum;
        }
    }

    /* Find the minimal depth such that len[1][depth] > n. */
    int depth = 0;
    while (depth < 64 && len[1][depth] <= n)
        ++depth;

    /* Retrieve the symbol at position n. */
    return get_sym(1, len, depth, n);
}
