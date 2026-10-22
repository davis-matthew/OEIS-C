/*
 * A395066  Triangle T(n,k), n >= 0, 0 <= k <= n, read by rows, where T(n,k) =
 *          [x^k] Product_{j=0..n} (1 + j^4*x).
 * https://oeis.org/A395066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth element of the triangle T(n,k) defined by
   T(0,0) = 1,
   T(n,k) = T(n-1,k) + n^4 * T(n-1,k-1)   (n ≥ 1, 0 ≤ k ≤ n),
   read row‑by‑row starting at row 0.  The function below computes
   a(n) in O(r·k) time where r is the row index of n. */
#include <math.h>
#include <stdint.h>   /* for uint64_t used by idx_t (if needed) */

#define MAX_K 200    /* sufficient for all n ≤ 10000 (k ≤ 140) */

seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to index n. */
    idx_t r = (idx_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);
    idx_t k = n - r * (r + 1) / 2;

    /* Dynamic‑programming array (no dynamic allocation). */
    seq_t arr[MAX_K];
    arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= k && i < MAX_K; ++i)
        arr[i] = (seq_t)0;

    /* Build rows 1 … r of the triangle. */
    for (idx_t i = 1; i <= r; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t i4 = (i_seq * i_seq) * (i_seq * i_seq);  /* i^4 */
        idx_t jmax = (i < k ? i : k);
        for (idx_t j = jmax; j > 0; --j)
            arr[j] = arr[j] + i4 * arr[j - 1];
    }

    return arr[k];
}
