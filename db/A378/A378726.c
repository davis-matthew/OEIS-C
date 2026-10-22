/*
 * A378726  The total number of fires on a rooted undirected infinite ternary
 *          tree with a self-loop at the root, when the chip-firing process
 *          starts with 3n chips at the root.
 * https://oeis.org/A378726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of chip firings on an infinite ternary tree
 *            with a self-loop at the root, starting with 3*n chips at the
 *            root
 * Range:     u8 n <= 61, u16 n <= 6196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

#define MAX_DEPTH 200

/* Compute 3^d for 0 <= d < MAX_DEPTH */
static seq_t pow3[MAX_DEPTH];
static int pow3_initialized = 0;

seq_t a(idx_t n)
{
    static seq_t C[MAX_DEPTH];
    size_t i;

    /* Initialize powers of 3 once */
    if (!pow3_initialized) {
        pow3[0] = (seq_t)1;
        for (i = 1; i < MAX_DEPTH; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;
        pow3_initialized = 1;
    }

    /* clear per-vertex chip counts for each depth */
    for (i = 0; i < MAX_DEPTH; ++i)
        C[i] = (seq_t)0;

    /* initial chips at the root */
    C[0] = (seq_t)3 * (seq_t)n;

    seq_t total_fires = (seq_t)0;

    for (;;) {
        /* find deepest depth with at least 4 chips per vertex */
        size_t d;
        for (d = MAX_DEPTH - 1; d != (size_t)(-1); --d)
            if (C[d] >= (seq_t)4)
                break;
        if (d == (size_t)(-1)) /* none found */
            break;

        if (d == 0) {
            /* root */
            seq_t r = (C[0] - (seq_t)4) / (seq_t)3 + (seq_t)1;
            total_fires += r;
            C[0] -= r * (seq_t)3;          /* net loss of 3 per fire */
            C[1] += r;                     /* each child receives 1 per fire */
        } else {
            /* vertex at depth d >= 1 */
            seq_t r = (C[d] - (seq_t)4) / (seq_t)4 + (seq_t)1;
            total_fires += r * pow3[d];    /* number of vertices at depth d */

            C[d]     -= r * (seq_t)4;      /* each vertex loses 4 per fire */
            C[d - 1] += r * (seq_t)3;      /* each parent receives 3 per fire */
            C[d + 1] += r;                 /* each child receives 1 per fire */
        }
    }

    return total_fires;
}
