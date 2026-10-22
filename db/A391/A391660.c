/*
 * A391660  Square array read by antidiagonals: T(n,k) is the Kruskal-Macaulay
 *          function K_n of degree n evaluated at k.
 * https://oeis.org/A391660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) of the
 *            Kruskal–Macaulay function K_n(k) by antidiagonals. Here T(n,k) =
 *            K_n(k) (for n ≥ 1 and k ≥ 0)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*--------------------------------------------------------------------
   Utility: binomial coefficient C(n,k) for small arguments.
   Arguments are non‑negative and n is assumed small (≤ 200 in
   practice for this sequence).  The result always fits in seq_t.
--------------------------------------------------------------------*/
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i; /* exact division */
    }
    return res;
}

/*--------------------------------------------------------------------
   Kruskal–Macaulay function K_d(k).
   It is defined via the combinatorial number system:
   write k = Σ C(a_i, i) with a_d > a_{d-1} > … > a_1 ≥ 0.
   Then K_d(k) = Σ C(a_i, i‑1).
   The implementation computes the representation iteratively.
--------------------------------------------------------------------*/
static seq_t kruskal_macaulay(idx_t k, idx_t d)
{
    seq_t result = 0;
    seq_t rem = (seq_t)k;
    idx_t cur = d;
    while (cur > 0 && rem > 0) {
        /* find largest x with C(x,cur) <= rem */
        idx_t x = cur;                 /* minimal possible upper index */
        while (true) {
            seq_t val = binom_small(x + 1, cur);
            if (val <= rem) ++x;
            else break;
        }
        result += binom_small(x, cur - 1);
        rem -= binom_small(x, cur);
        --cur;
    }
    /* when rem == 0 the remaining lower terms contribute 0 */
    return result;
}

/*--------------------------------------------------------------------
   Main sequence function.
   Given n ≥ 1 (offset 1), it locates the corresponding antidiagonal
   position, retrieves the appropriate row (degree) and column (k),
   and returns K_row(col).
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range, safety */

    /* find s such that s*(s+1)/2 >= n  (antidiagonal index) */
    idx_t s = 1;
    while (((unsigned __int128)s * (s + 1)) / 2 < n) ++s;

    /* t_prev = (s-1)*s/2  (number of elements before this antidiagonal) */
    idx_t t_prev = (idx_t)((((unsigned __int128)(s - 1) * s)) / 2);

    idx_t row = n - t_prev;            /* degree (≥1) */
    idx_t col = s - row;               /* k  (≥0) */

    if (col == 0) return 0;            /* K_d(0) = 0 for all d */

    return kruskal_macaulay(col, row);
}
