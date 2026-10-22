/*
 * A321379  Number of ways to write n as n = a*b*c*d with 1 < a <= b <= c <= d
 *          < n.
 * https://oeis.org/A321379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as * a*b*c*d with 1 < a <= b <=
 *            c <= d < n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmpidx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmpidx.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    /* Generate all divisors of n (a <= n). */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[divcnt++] = i;
            if (i != n / i) divs[divcnt++] = n / i;
            if (divcnt >= sizeof(divs)/sizeof(divs[0])) break; /* safety */
        }
    }
    qsort(divs, divcnt, sizeof(idx_t), cmpidx);

    seq_t count = 0;

    /* Loop over a <= b <= c, compute d = n/(a*b*c). */
    for (size_t ii = 1; ii < divcnt; ++ii) {          /* a > 1 */
        idx_t a_val = divs[ii];
        for (size_t jj = ii; jj < divcnt; ++jj) {    /* b >= a */
            idx_t b_val = divs[jj];
            idx_t prod_ab = a_val * b_val;
            if (prod_ab > n) break;
            for (size_t kk = jj; kk < divcnt; ++kk) { /* c >= b */
                idx_t c_val = divs[kk];
                idx_t prod_abc = prod_ab * c_val;
                if (prod_abc > n) break;
                if (n % prod_abc != 0) continue;
                idx_t d_val = n / prod_abc;
                if (d_val < c_val) continue;        /* enforce d >= c */
                if (d_val == n) continue;           /* d < n enforced */
                count++;
            }
        }
    }

    return count;
}
