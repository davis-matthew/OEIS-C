/*
 * A318485  Number of p-trees of weight 2n + 1 in which all outdegrees are
 *          odd.
 * https://oeis.org/A318485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts p‑trees of weight 2n+1 with all odd outdegrees
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* weight of the tree for index n */
    idx_t Wmax = (idx_t)((seq_t)n * 2 + 1);

    /* dynamic arrays of size Wmax+1, zero‑initialised */
    seq_t *ev = (seq_t *)calloc((size_t)(Wmax + 1), sizeof(seq_t)); /* even‑parity partitions */
    seq_t *od = (seq_t *)calloc((size_t)(Wmax + 1), sizeof(seq_t)); /* odd‑parity partitions */
    seq_t *dp = (seq_t *)calloc((size_t)(Wmax + 1), sizeof(seq_t)); /* a(n) values for odd weights */

    if (!ev || !od || !dp) {
        /* memory allocation failed – return 0 (undefined behaviour otherwise) */
        return (seq_t)0;
    }

    ev[0] = (seq_t)1;  /* empty partition has even number of parts (0) */
    od[0] = (seq_t)0;
    dp[1] = (seq_t)1;  /* base tree: single node, weight 1 */

    /* process part weight 1 */
    for (idx_t w = 1; w <= Wmax; ++w) {
        ev[w] += dp[1] * od[w - 1];
        od[w] += dp[1] * ev[w - 1];
    }

    /* process odd parts > 1 */
    for (idx_t v = 3; v <= Wmax; v += 2) {
        /* number of trees of weight v */
        dp[v] = od[v];

        /* update partition counts to include part weight v */
        for (idx_t w = v; w <= Wmax; ++w) {
            ev[w] += dp[v] * od[w - v];
            od[w] += dp[v] * ev[w - v];
        }
    }

    seq_t result = dp[Wmax];

    free(ev);
    free(od);
    free(dp);

    return result;
}
