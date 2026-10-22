/*
 * A361471  Size of the symmetric difference of {1,2,3,4}, {2,4,6,8}, ...,
 *          {n,2n,3n,4n}.
 * https://oeis.org/A361471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Symmetric difference size: a(n) = |{1 ≤ k ≤ 4n | k belongs to an odd
   number of sets {i,2i,3i,4i} for 1 ≤ i ≤ n}| */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle out‑of‑offset calls gracefully. */
    if (n == 0U)
        return (seq_t)0;

    /* Numbers in the union of all sets lie in 1 … 4n. */
    idx_t maxk = 4 * n;
    size_t len = (size_t)(maxk + 1);          /* +1 for 1‑based indexing */

    /* Allocate a parity array: 0 = even count, 1 = odd count. */
    unsigned char *par = (unsigned char *)malloc(len);
    if (!par)
        return (seq_t)0;  /* allocation failure → zero result */

    memset(par, 0, len);

    /* Toggle membership for each set {i,2i,3i,4i}. */
    for (idx_t i = 1; i <= n; ++i) {
        par[i]     ^= 1;
        par[2 * i] ^= 1;
        par[3 * i] ^= 1;
        par[4 * i] ^= 1;
    }

    /* Count how many positions have odd parity. */
    seq_t count = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        if (par[k])
            ++count;
    }

    free(par);
    return count;
}
