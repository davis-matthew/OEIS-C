/*
 * A250261  Number A(n,k) of permutations p of [n] such that p(i) > p(i+1) iff
 *          i = 1 + k*m for some m >= 0; square array A(n,k), n>=0, k>=0, read
 *          by antidiagonals.
 * https://oeis.org/A250261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 101, u32 n <= 186, u64 n <= 402,
 *            u128 n <= 899, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A(i,j) where
 *   the pair (i,j) is obtained by reading the square array
 *   A(n,k) by antidiagonals.  A(n,k) counts permutations
 *   p of [n] such that p(i) > p(i+1) iff i = 1 + k*m for some m≥0.
 *   For given (i,j), the pattern of descents is:
 *       descent at positions i where (pos-1) is divisible by j (j>0),
 *       ascent otherwise.  For j=0 or j≥i-1 the pattern reduces to
 *       a single descent at position 1.  A(n,k) is the
 *       number of permutations of [n] satisfying that pattern.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Allocate an array of seq_t elements.  Handles C and C++ builds. */
static inline seq_t* alloc_seq_array(size_t n)
{
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}
static inline void free_seq_array(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* Count permutations of [len] whose descent set is exactly
 * { pos | 1 ≤ pos ≤ len-1 and (pos-1) % k_eff == 0 }.
 * For k_eff = 0 does not occur (len ≥ 2 here).
 */
static seq_t count_pattern(idx_t len, idx_t k_eff)
{
    if (len <= 1) return (seq_t)1;

    size_t cap = (size_t)len + 1;      /* indices 1..len */
    seq_t *dp_prev = alloc_seq_array(cap);
    seq_t *dp_cur  = alloc_seq_array(cap);
    seq_t *prefix  = alloc_seq_array(cap);

    /* Base case: length 1 */
    for (idx_t i = 1; i <= len; ++i) dp_prev[i] = (seq_t)0;
    dp_prev[1] = (seq_t)1;

    for (idx_t i = 2; i <= len; ++i) {
        idx_t pos = i - 1;             /* comparison index (1‑based) */
        int is_desc = (((pos - 1) % k_eff) == 0);  /* pattern '>' */

        /* Prefix sums of dp_prev[1..i-1] */
        prefix[0] = (seq_t)0;
        for (idx_t m = 1; m <= i - 1; ++m)
            prefix[m] = prefix[m - 1] + dp_prev[m];

        /* Compute dp_cur using prefix sums */
        for (idx_t j = 1; j <= i; ++j) {
            if (is_desc) {
                /* descent: sum_{k=j}^{i-1} dp_prev[k] */
                dp_cur[j] = prefix[i - 1] - prefix[j - 1];
            } else {
                /* ascent: sum_{k=1}^{j-1} dp_prev[k] */
                dp_cur[j] = prefix[j - 1];
            }
        }

        /* Swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    /* Sum over last DP row to get total count */
    seq_t total = (seq_t)0;
    for (idx_t j = 1; j <= len; ++j)
        total += dp_prev[j];

    free_seq_array(dp_prev);
    free_seq_array(dp_cur);
    free_seq_array(prefix);

    return total;
}

/* Compute A(n,k) according to the definition. */
static seq_t A(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* For k = 0 or k ≥ n-1, the descent set reduces to {1}. */
    idx_t k_eff = (k == 0 || k >= n - 1) ? (n - 1) : k;
    return count_pattern(n, k_eff);
}

/* Main function requested by the problem statement.  Maps the
 * antidiagonal index n to a pair (i,k) and returns A(i,k).  */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that n lies in it. */
    idx_t d = 0;
    while (((d + 1) * (d + 2) / 2) <= n) ++d;

    idx_t start  = d * (d + 1) / 2;
    idx_t offset = n - start;

    idx_t i = offset;          /* n in the array A(n,k) */
    idx_t k = d - offset;      /* k in the array A(n,k) */

    return A(i, k);
}
