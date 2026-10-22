/*
 * A368197  Triangle read by rows: T(n,k) = Sum_{z=1..n} Sum_{y=1..n}
 *          Sum_{x=1..n} [GCD(f(x,y,z), n) = k], where f(x,y,z) = x^2 + y^2 -
 *          z^2.
 * https://oeis.org/A368197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) are obtained from the n‑th position in
 *            the triangle T(n,k). T(n,k) counts triples (x,y,z) with 1 <=
 *            x,y,z <= n and gcd(x^2 + y^2 - z^2 , n) = k
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;          /* flattened triangle values   */
    static size_t terms_len = 0;          /* number of elements stored  */
    static size_t terms_cap = 0;          /* allocated capacity         */
    static idx_t  rows_done = 0;          /* number of fully processed rows */

    if (n == 0)               /* offset is 1; never called with 0 */
        return 0;

    /* Grow the triangle until it contains at least n terms */
    while (terms_len < (size_t)n) {
        idx_t r = ++rows_done;          /* next row to compute   */
        size_t r_usize = (size_t)r;

        /* Count array for the current row (1..r) */
        seq_t *cnt = (seq_t*)calloc(r_usize + 1, sizeof(seq_t));
        seq_t *sq  = (seq_t*)calloc(r_usize + 1, sizeof(seq_t));
        if (!cnt || !sq) {              /* allocation failure    */
            free(cnt); free(sq);
            return 0;
        }

        /* Pre‑compute squares 1^2 … r^2 */
        for (idx_t i = 1; i <= r; ++i) {
            seq_t ii = (seq_t)i;
            sq[(size_t)i] = ii * ii;
        }

        /* Triple loop over all (x,y,z) and count gcd's */
        for (idx_t x = 1; x <= r; ++x) {
            seq_t sx = sq[(size_t)x];
            for (idx_t y = 1; y <= r; ++y) {
                seq_t sumxy = sx + sq[(size_t)y];
                for (idx_t z = 1; z <= r; ++z) {
                    seq_t diff;
                    if (sumxy > sq[(size_t)z])
                        diff = sumxy - sq[(size_t)z];
                    else
                        diff = sq[(size_t)z] - sumxy;

                    seq_t d = gcd(diff, (seq_t)r);
                    cnt[(size_t)d] += (seq_t)1;
                }
            }
        }

        /* Ensure enough space in the flat array */
        if (terms_len + r > terms_cap) {
            size_t newcap = terms_cap ? terms_cap << 1 : 32;
            while (newcap < terms_len + r) newcap <<= 1;
            terms = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
            terms_cap = newcap;
        }

        /* Append counts of the current row to the flat array */
        for (size_t k = 1; k <= r_usize; ++k)
            terms[terms_len++] = cnt[(size_t)k];

        free(cnt);
        free(sq);
    }

    return terms[(size_t)(n - 1)];
}
