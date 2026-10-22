/*
 * A347147  Square array read by antidiagonals: T(n,k) is the number of rook
 *          paths from (1,1) to (n,k) if the rook may travel 1 to i squares
 *          along rank or file i, n >= 1, k >= 1.
 * https://oeis.org/A347147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value T(i,j) read along antidiagonals of the square
 *            array T(n,k) where T(1,1) = 1, for n,k >= 1: T(n,k) =
 *            Σ_{i=max(1,n-k)}^{n-1} T(i,k) + Σ_{j=max(1,k-n)}^{k-1} T(n,j).
 *            The sequence enumerates T(i,j) with i+j-1 = s, and within each
 *            antidiagonal i increases from 1 to s-1
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static size_t T_dim = 0;          /* current maximal dimension (>= max n,k) */
static seq_t *T = NULL;           /* flat array of size (T_dim+1)*(T_dim+1) */

/* macro to access table element (lvalue) */
#define T_AT(n,k) (T[(n)*(T_dim+1)+(k)])

static void build_table(size_t newDim)
{
    /* allocate new table */
    seq_t *newT = (seq_t *)calloc((newDim + 1) * (newDim + 1), sizeof(seq_t));
    if (!newT) exit(1);          /* out of memory */
    if (T) free(T);
    T = newT;
    T_dim = newDim;

    /* compute values */
    for (idx_t n = 1; n <= (idx_t)newDim; ++n) {
        for (idx_t k = 1; k <= (idx_t)newDim; ++k) {
            if (n == 1 && k == 1) {
                T_AT(n, k) = 1;
            } else {
                seq_t sum = 0;
                idx_t i_start = (n > k) ? (n - k) : 1;
                for (idx_t i = i_start; i <= n - 1; ++i) {
                    sum += T_AT(i, k);
                }
                idx_t j_start = (k > n) ? (k - n) : 1;
                for (idx_t j = j_start; j <= k - 1; ++j) {
                    sum += T_AT(n, j);
                }
                T_AT(n, k) = sum;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1, but graceful for 0 */
    /* determine diagonal s such that (s-1)*s/2 < n <= s*(s+1)/2 */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* indices on this diagonal */
    idx_t prevCount = (s - 2) * (s - 1) / 2;   /* number of terms before this diagonal */
    idx_t offset   = n - prevCount - 1;        /* 0‑based offset within diagonal */
    idx_t i = offset + 1;                      /* row index (1‑based) */
    idx_t j = s - 1 - offset;                  /* column index (1‑based) */

    /* ensure table large enough */
    size_t requiredDim = (size_t)(s - 1);
    if (requiredDim > T_dim) build_table(requiredDim);

    return T_AT(i, j);
}
