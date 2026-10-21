/*
 * A255874  Triangular array T: T(n,k) = number of subset S of {1,2,...,n+1}
 *          such that |S| > 1 and max(S*) = k, where S* is the set {x(2)-x(1),
 *          x(3)-x(2), ..., x(h+1)-x(h)} when the elements of S are written as
 *          x(1) < x(2) < ... < x(h+1).
 * https://oeis.org/A255874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255874:  a(n) = number of subsets S of {1,…,r+1} with |S|>1
   and max(S*)=k, where (r,k) is the pair corresponding to index n.
   T(r,k) = f(r,k) – f(r,k‑1),  f(r,k) = (r+1)*A – B, with
   A = Σ_{s=1}^{r} m_s,   B = Σ_{s=1}^{r} s·m_s,
   m_s = number of compositions of s into parts in [1,k]   (k≥1).       */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to compute f(n,k) */
static seq_t *prefix_buf = NULL;      /* dynamic buffer for prefix sums   */
static idx_t  prefix_cap = 0;         /* current capacity of prefix_buf   */

static void ensure_prefix_cap(idx_t n)
{
    if (prefix_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(prefix_buf, (n + 1) * sizeof(seq_t));
        /* If realloc fails, we keep the old buffer; the function
           will still work for smaller n.  In typical usage it never
           fails because n never grows beyond the values seen.      */
        if (tmp) {
            prefix_buf = tmp;
            prefix_cap = n + 1;
        }
    }
}

/* f(n,k) – number of subsets S⊆{1..n+1} with |S|>1 and all gaps ≤k   */
static seq_t compute_f(idx_t N, idx_t k)
{
    if (N == 0 || k == 0) return 0;

    ensure_prefix_cap(N);
    seq_t *p = prefix_buf;   /* p[s] = prefix sum of m[0..s] */

    p[0] = 1;                /* m[0] = 1 (empty composition)      */
    seq_t A = 0;              /* Σ m[s] for s=1..N                 */
    seq_t B = 0;              /* Σ s·m[s]                           */

    for (idx_t s = 1; s <= N; ++s) {
        seq_t val = p[s - 1];          /* prefix[s-1]                     */
        if (s > k)                     /* subtract prefix[s-k-1]          */
            val -= p[s - k - 1];
        /* val = m[s] ----------------------------------------------*/
        p[s] = p[s - 1] + val;         /* update prefix[s]                 */
        A += val;
        B += (seq_t)s * val;
    }

    return (seq_t)(N + 1) * A - B;      /* f(N,k)                           */
}

/* Find row r such that r(r+1)/2 ≥ n  (first time this inequality holds) */
static idx_t find_row(idx_t n)
{
    idx_t r = (idx_t)((sqrtl((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;
    return r;
}

/* Public sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const idx_t row = find_row(n);
    const idx_t col = n - (row - 1) * row / 2;   /* column index, 1‑based   */

    const seq_t cur  = compute_f(row, col);
    const seq_t prev = compute_f(row, col - 1);

    return cur - prev;
}
