/*
 * A346036  Number of swaps needed to return the recursive transform T(k) =
 *          concatenate(reverse(k), len(k) + 1) to the tuple 1, 2, ..., n.
 * https://oeis.org/A346036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - cycles(T(n)) where T is defined recursively:
 *            T(1)=[1]; T(k)=reverse(T(k-1)) with k appended. The mapping from
 *            element e to position pos(e) in T(n) satisfies: let d = n - e;
 *            if d is odd: pos(e) = (d + 1) / 2; else: pos(e) = e + d / 2. The
 *            function returns the number of swaps needed to s...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle offset < 1 */
    if (n < 1) return (seq_t)0;

    /* cast to idx_t for indexing */
    idx_t N = (idx_t)n;

    /* allocate auxiliary arrays */
    idx_t *pos = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    unsigned char *vis = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    if (!pos || !vis) /* fallback on allocation failure */
    {
        free(pos);
        free(vis);
        return (seq_t)0;
    }

    /* compute position array using the closed form */
    for (idx_t e = 1; e <= N; ++e)
    {
        idx_t d = N - e;
        if (d & 1)
            pos[e] = (d + 1) / 2;
        else
            pos[e] = e + d / 2;
    }

    /* count cycles of the permutation e -> pos[e] */
    idx_t cycles = 0;
    for (idx_t i = 1; i <= N; ++i) vis[i] = 0;
    for (idx_t i = 1; i <= N; ++i)
    {
        if (!vis[i])
        {
            ++cycles;
            idx_t j = i;
            do
            {
                vis[j] = 1;
                j = pos[j];
            } while (!vis[j]);
        }
    }

    free(pos);
    free(vis);

    /* minimal swaps = n - number of cycles */
    seq_t result = (seq_t)(N - cycles);
    return result;
}
