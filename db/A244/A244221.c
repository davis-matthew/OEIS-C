/*
 * A244221  Parity of Greedy Catalan Base representation for n: a(n) =
 *          A014418(n) reduced modulo 2.
 * https://oeis.org/A244221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014418(n) mod 2 (parity of the greedy Catalan base
 *            representation)
 * Range:     u8 n <= 41, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy initialization of Catalan numbers */
    static seq_t *cat = NULL;
    static idx_t cat_len = 0;
    static idx_t cat_cap = 0;

    if (!cat) {
        /* initial capacity */
        cat_cap = 16;
        cat = (seq_t *)malloc(cat_cap * sizeof(seq_t));
        /* C0 = 1 */
        cat[0] = (seq_t)1;
        cat_len = 1;
    }

    /* Ensure Catalan numbers are generated up to > n */
    seq_t n_seq = (seq_t)n;
    while (cat_len > 0 && cat[cat_len - 1] <= n_seq) {
        if (cat_len >= cat_cap) {
            cat_cap <<= 1;
            cat = (seq_t *)realloc(cat, cat_cap * sizeof(seq_t));
        }
        /* Compute next Catalan number: C_{k+1} = C_k * 2*(2*k+1) / (k+2) */
        idx_t k = cat_len - 1;                 /* current index of last computed C_k */
        seq_t prev = cat[k];
        /* Use seq_t arithmetic to avoid overflow of the intermediate integer */
        seq_t num = prev * (seq_t)(2 * (2 * k + 1));
        seq_t denom = (seq_t)(k + 2);
        seq_t next = num / denom;
        cat[cat_len++] = next;
    }

    /* Greedy subtraction of Catalan numbers >= 2 */
    seq_t r = n_seq;
    while (r >= (seq_t)2) {
        /* Binary search for largest Catalan ≤ r */
        idx_t lo = 0, hi = cat_len - 1, pos = 0;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            if (cat[mid] <= r) {
                pos = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        r -= cat[pos];
    }
    return r;      /* 0 or 1 */
}
