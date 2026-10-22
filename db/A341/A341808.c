/*
 * A341808  Number of ways to write n as an ordered sum of 9 nonzero
 *          tetrahedral numbers.
 * https://oeis.org/A341808
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 9‑tuples of non‑zero tetrahedral
 *            numbers summing to n. Computed by a 2‑dimensional DP:
 *            ways[parts][sum]
 * Range:     u8 n <= 23, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 9) return 0;

    const size_t N = (size_t)n;

    /* Collect all tetrahedral numbers T_k = k(k+1)(k+2)/6 <= n. */
    seq_t T[200];   /* enough for n up to several million */
    size_t tlen = 0;
    for (idx_t k = 1;; ++k) {
        seq_t tk = (seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2) / 6;
        if (tk > n) break;
        T[tlen++] = tk;
    }

    /* Allocate DP arrays.  prev[s] = number of ways to reach sum s
       using part-1 numbers; curr[s] does it for part numbers.  */
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!prev || !curr) return 0;       /* out of memory → error */

    prev[0] = 1;                         /* empty sum for 0 parts */

    for (size_t part = 1; part <= 9; ++part) {
        /* reset curr array to 0 */
        for (size_t i = 0; i <= N; ++i) curr[i] = 0;

        for (size_t s = 0; s <= N; ++s) if (prev[s] != 0) {
            for (size_t i = 0; i < tlen; ++i) {
                size_t ns = s + (size_t)T[i];
                if (ns > N) break;        /* T sorted ascending */
                curr[ns] += prev[s];
            }
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[N];

    free(prev);
    free(curr);
    return result;
}
