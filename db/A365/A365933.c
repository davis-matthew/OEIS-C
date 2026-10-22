/*
 * A365933  a(n) is the period of the remainders when repdigits are divided by
 *          n.
 * https://oeis.org/A365933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 * L, where L is the period of the remainders of
 *            repunits modulo n. For n = 1, the period is defined to be 1
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset case explicitly. */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)0) return (seq_t)0;          /* safety: undefined for 0 */

    /* Allocate an array to remember when each remainder first appeared. */
    idx_t *visited = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!visited) return (seq_t)0;                /* allocation failure */

    for (idx_t i = 0; i < n; ++i) visited[i] = 0;

    /* Start with the remainder of the first repunit (1). */
    idx_t r   = 1 % n;          /* r₁ */
    idx_t pos = 1;               /* position counter (1‑based) */
    visited[r] = pos;

    for (;;) {
        /* Compute the next remainder: r_{k+1} = (r_k * 10 + 1) mod n.
           Use 128‑bit arithmetic to avoid overflow for very large n. */
        r = (idx_t)(((__int128)r * 10 + 1) % n);
        ++pos;
        if (visited[r] != 0) {          /* repetition found */
            idx_t period = pos - visited[r];   /* length of the cycle */
            free(visited);
            return (seq_t)(period * (idx_t)9);
        }
        visited[r] = pos;
    }
}
