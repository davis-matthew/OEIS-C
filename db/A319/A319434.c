/*
 * A319434  Take Golomb's sequence A001462 and shorten all the runs by 1.
 * https://oeis.org/A319434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001462(A001462(A001462(n) + n) + n)
 * Range:     u8 n <= 5579, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *G_arr = NULL;   /* 0‑based array: G_arr[0] unused, G_arr[1] = 1 */
static idx_t G_cap = 0;       /* current allocation size (0 … G_cap‑1) */

/* Ensure G_arr contains the Golomb values G(k) for every k <= up_to (inclusive). */
static void ensure_G(idx_t up_to)
{
    if (up_to < G_cap) return;          /* already computed */

    size_t new_cap = (size_t)up_to + 1;  /* include index up_to */
    seq_t *tmp = (seq_t *)realloc(G_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);       /* allocation failure */
    G_arr = tmp;

    /* compute new elements starting from the old limit */
    for (size_t i = G_cap; i < new_cap; ++i)
    {
        if (i == 0)
        {
            G_arr[i] = (seq_t)0;         /* unused sentinel */
        }
        else if (i == 1)
        {
            G_arr[i] = (seq_t)1;         /* G(1) = 1 */
        }
        else
        {
            /* Recurrence: G(k) = 1 + G(k - G(G(k-1))) */
            idx_t prev = (idx_t)G_arr[i - 1];              /* G(k-1) */
            idx_t idx  = (idx_t)i - (idx_t)G_arr[prev];    /* k - G(G(k-1)) */
            G_arr[i] = (seq_t)1 + G_arr[(size_t)idx];
        }
    }
    G_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not used; offset starts at 1 */

    /* Step 1: G(n) */
    ensure_G(n);
    seq_t g1 = G_arr[(size_t)n];            /* G(n) */

    /* Step 2: G(n + G(n)) */
    idx_t s1 = n + (idx_t)g1;               /* n + G(n) */
    ensure_G(s1);
    seq_t g2 = G_arr[(size_t)s1];           /* G(n + G(n)) */

    /* Step 3: G(n + G(n + G(n))) */
    idx_t s2 = n + (idx_t)g2;               /* n + G(n + G(n)) */
    ensure_G(s2);
    return G_arr[(size_t)s2];               /* a(n) = G(G(G(n)+n)+n) */
}
