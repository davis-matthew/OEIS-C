/*
 * A282905  Concatenation of the sums of elements of P{1}, P{1, 2}, P{1, 2,
 *          3}, ..., P{1, 2, 3, ..., n}; where P{A} denotes the power set of
 *          set A ordered by the size of the subsets, and in each subset,
 *          following the increasing order.
 * https://oeis.org/A282905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth element of the sequence formed by listing, in
 *            order of increasing subset size and then lexicographically, the
 *            sums of the non‑empty subsets of {1,2,…,k} for k = 1,2,3,…
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* recursive search for the offset‑th sum among all r‑element subsets
   of {1,…,k}.  *ofs is decremented each time a combination is skipped.
   When *ofs becomes zero, the corresponding sum is written to *res
   and true is returned to unwind the recursion. */
static bool rec_find(idx_t k, idx_t r, idx_t start, seq_t sum,
                     idx_t *ofs, seq_t *res)
{
    if (r == 0) {
        if (*ofs == 0) {
            *res = sum;
            return true;
        }
        (*ofs)--;
        return false;
    }
    for (idx_t i = start; i <= k - r + 1; ++i) {
        if (rec_find(k, r - 1, i + 1, sum + (seq_t)i, ofs, res))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* find the smallest k such that the total number of sums up to
       set {1,…,k} is at least n */
    idx_t k = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++k;
        cumulative += (((idx_t)1 << k) - 1);
    }

    /* previous total (up to set {1,…,k-1}) */
    idx_t prev_cum = cumulative - ((((idx_t)1 << k) - 1));
    /* offset within the sums of set {1,…,k} (zero‑based) */
    idx_t offset = n - prev_cum - 1;

    seq_t result = 0;
    idx_t ofs = offset;
    /* search over all subset sizes */
    for (idx_t r = 1; r <= k; ++r) {
        if (rec_find(k, r, 1, (seq_t)0, &ofs, &result))
            break;
    }
    return result;
}
