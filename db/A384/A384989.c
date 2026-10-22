/*
 * A384989  Order of the permutation of [n] formed by a Josephus elimination
 *          variation: take 3, skip 1.
 * https://oeis.org/A384989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 69, u32 n <= 341, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = order of the permutation on [n] produced by the Josephus
 * variation “take 3, skip 1”.  The permutation is obtained by repeatedly
 * taking the next three remaining elements in the current circular order,
 * outputting them, then moving the following element to the back of the
 * circle.  The order of the permutation (the least common multiple of its
 * cycle lengths) is returned.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    size_t N = (size_t)n;          /* safe for n up to about 10 000 */

    /* Circular buffer holding the remaining elements */
    idx_t *buf = (idx_t *)malloc(N * sizeof(idx_t));
    if (!buf) return (seq_t)0;
    for (size_t i = 0; i < N; ++i) buf[i] = (idx_t)(i + 1);

    /* Record the elimination order */
    idx_t *order = (idx_t *)malloc(N * sizeof(idx_t));
    if (!order) { free(buf); return (seq_t)0; }

    size_t head = 0;   /* next position to pop */
    size_t size = N;   /* current number of elements in buf */
    size_t out  = 0;   /* number of elements output so far */

    while (size > 0) {
        size_t take = size >= 3 ? 3 : size;
        for (size_t t = 0; t < take; ++t) {
            idx_t v = buf[head];
            head = (head + 1) % N;
            --size;
            order[out++] = v;
        }
        if (size == 0) break;

        /* Skip one element: pop it, then push it to the back of the buffer */
        idx_t v = buf[head];
        head = (head + 1) % N;
        --size;
        buf[(head + size) % N] = v;   /* push back */
        ++size;
    }

    /* Map each element to the position where it was removed (1‑based) */
    idx_t *pos = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    if (!pos) { free(buf); free(order); return (seq_t)0; }
    for (size_t i = 0; i < N; ++i) {
        idx_t e = order[i];
        pos[e] = (idx_t)(i + 1);
    }

    /* Compute the order of the permutation (LCM of cycle lengths) */
    bool *vis = (bool *)calloc(N + 1, sizeof(bool));
    if (!vis) { free(buf); free(order); free(pos); return (seq_t)0; }

    seq_t l = (seq_t)1;
    for (idx_t e = 1; e <= (idx_t)N; ++e) {
        if (vis[e]) continue;
        idx_t cur = e;
        size_t clen = 0;
        do {
            vis[cur] = true;
            cur = pos[cur];
            ++clen;
        } while (cur != e);
        seq_t g = gcd(l, (seq_t)clen);
        l = (l / g) * (seq_t)clen;
    }

    free(buf);
    free(order);
    free(pos);
    free(vis);
    return l;
}
