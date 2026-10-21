/*
 * A258376  Number of edges connecting the subgraph on {1, ..., n} with the
 *          complement in the minimal graph on positive natural numbers where
 *          degree(n) equals n.
 * https://oeis.org/A258376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} max(0, A005206(i)-n+i)
 * Range:     u8 n <= 45, u16 n <= 743, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A005206(i) is the Hofstadter G sequence: G(1)=1, G(n)=n-G(G(n-1)) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *G = NULL;   /* cache for G(1..max) */
static idx_t Gmax = 0;    /* largest index cached */

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, defined as 0 */

    /* Ensure G[1..n] are cached. */
    if (n > Gmax) {
        idx_t newlen = n + 1;   /* +1 so that index n is accessible */
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(G, newlen * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(G, newlen * sizeof(seq_t));
#endif
        if (!tmp) exit(EXIT_FAILURE);
        G = tmp;

        for (idx_t i = Gmax + 1; i <= n; ++i) {
            if (i == 1) {
                G[1] = 1;
            } else {
                idx_t gprev = (idx_t)G[i - 1];
                G[i] = (seq_t)i - G[gprev];
            }
        }
        Gmax = n;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t gi = G[i];
        seq_t threshold = (seq_t)(n - i);   /* n - i  (idx_t → seq_t) */
        if (gi > threshold) {
            sum += gi - threshold;
        }
    }
    return sum;
}
