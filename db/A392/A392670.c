/*
 * A392670  Triangle read by rows where row n is the numerators of the node
 *          values in a preorder traversal of the Calkin-Wilf tree with n
 *          levels, columns 0 <= k < 2^n-1.
 * https://oeis.org/A392670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, frac
 * Formula:   a(n) is the numerator of the kth node in the preorder *
 *            traversal of a perfect binary tree of depth R, where R is the *
 *            smallest integer such that Σ_{i=1}^{R}(2^i−1) ≥ n and * k = n −
 *            Σ_{i=1}^{R−1}(2^i−1). The tree is the Calkin–Wilf tree. * The
 *            recursive descent follows the rule that a left chil...
 * Range:     u8 n <= 9375, u16 n <= 32752, u32 n <= 32752, u64 n <= 32752,
 *            u128 n <= 32752, big any n
 * Validated: standard, pass, 136 terms through n = 9997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Recursive helper: returns the numerator of the node that is the
 * k‑th element (1‑based) in the preorder traversal of a perfect
 * binary tree of given depth.  The current node has fraction a/b.
 */
static seq_t get_kth(idx_t depth, idx_t k, seq_t a, seq_t b)
{
    if (k == 1)            /* current node */
        return a;

    /* size of the left subtree (number of nodes excluding its root) */
    idx_t size_left = ((idx_t)1 << (depth - 1)) - 1;

    if (k <= 1 + size_left) {          /* node lies in the left subtree */
        seq_t new_a = a;
        seq_t new_b = a + b;
        return get_kth(depth - 1, k - 1, new_a, new_b);
    } else {                            /* node lies in the right subtree */
        seq_t new_a = a + b;
        seq_t new_b = b;
        return get_kth(depth - 1, k - 1 - size_left, new_a, new_b);
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* not defined for n=0, but return 0 */

    idx_t prev_total = 0;
    idx_t r = 0;
    idx_t row_size;

    /* Find the tree depth r such that the nth term lies in row r. */
    for (;;) {
        ++r;
        row_size = ((idx_t)1 << r) - 1;   /* size of row r = 2^r - 1 */
        idx_t new_total = prev_total + row_size;
        if (n <= new_total) {             /* nth term is in this row */
            idx_t k = n - prev_total;     /* its position within the row */
            return get_kth(r, k, (seq_t)1, (seq_t)1);
        }
        prev_total = new_total;
    }
}
