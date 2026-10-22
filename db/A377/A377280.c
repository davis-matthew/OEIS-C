/*
 * A377280  Given n cards, each time you reversing the order of the top 1, 2,
 *          3, ..., n-1, n cards, then repeat reversing 1, 2, 3, ... cards. Do
 *          reversing at least once. the minimum number of steps required to
 *          return all the cards to their original position.
 * https://oeis.org/A377280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A003558(n)
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the order of the permutation obtained by applying, in
   sequence, the reversals of the top 1, 2, 3, …, n cards (A003558),
   then multiply that order by n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Euclidean algorithm for seq_t (unsigned or arbitrary‑precision). */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Reusable static buffers. */
    static idx_t *perm = NULL;    /* destination of each original index   */
    static size_t perm_cap = 0;
    static bool *visited = NULL;  /* cycle visitation flag              */
    static size_t vis_cap = 0;

    if ((size_t)n > perm_cap) {
        free(perm);
        perm = (idx_t *)malloc((size_t)n * sizeof(idx_t));
        perm_cap = (size_t)n;
    }
    if ((size_t)n > vis_cap) {
        free(visited);
        visited = (bool *)malloc((size_t)n * sizeof(bool));
        vis_cap = (size_t)n;
    }

    /* Build the permutation P: P[i] = final position of card that
       started at position i. */
    for (idx_t i = 0; i < n; ++i) {
        idx_t pos = i;
        for (idx_t k = 1; k <= n; ++k) {
            if (pos < k)
                pos = k - 1 - pos;   /* reflect in the top k block */
        }
        perm[i] = pos;
    }

    /* Find the order of the permutation – the LCM of its cycle
       lengths. */
    seq_t lcm = 1;
    for (idx_t i = 0; i < n; ++i)
        visited[i] = false;

    for (idx_t i = 0; i < n; ++i) {
        if (visited[i])
            continue;

        /* Walk the cycle starting at i. */
        idx_t cur  = i;
        idx_t clen = 0;
        do {
            visited[cur] = true;
            cur = perm[cur];
            ++clen;
        } while (cur != i);

        if (clen > 1) {
            seq_t len_seq = (seq_t)clen;
            seq_t g = gcd_seq(lcm, len_seq);
            lcm = (lcm / g) * len_seq;
        }
    }

    /* Final answer: a(n) = n * A003558(n). */
    return (seq_t)n * lcm;
}
