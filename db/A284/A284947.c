/*
 * A284947  Irregular triangle read by rows: coefficients of the cycle
 *          polynomial of the n-complete graph K_n.
 * https://oeis.org/A284947
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 0 for n<3. For n≥3, let r = floor((sqrt(1+8n) - 5)/2). If
 *            r==0 or n > s + r - 1 where s = (r+2)(r+3)/2, then a(n)=0.
 *            Otherwise let d = n - s; k = 3 + d; graph size g = r + 2. Then
 *            a(n) = binomial(g, k) * Pochhammer(3, k-3), where Pochhammer(3,
 *            m) = 3·4·…·(m+2) (empty product = 1). This rea...
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) returns seq_t */

seq_t a(idx_t n) {
    if (n < 6) return 0;                /* <6 rows give 0 because r=0 or block empty */

    /* compute r = floor((sqrt(1+8n)-5)/2)  */
    double d = sqrt((double)(1 + 8 * (long double)n));
    idx_t r = (idx_t)((d - 5.0) / 2.0);
    /* correct rounding errors */
    if (r > 0) {
        while ((r + 2) * (r + 3) / 2 > n) r--;
        while ((r + 1 + 2) * (r + 1 + 3) / 2 <= n) r++;
    }

    idx_t s = (r + 2) * (r + 3) / 2;
    idx_t end = s + r - 1;
    if (r == 0 || n > end) return 0;    /* not within a block */

    idx_t dpos = n - s;                  /* offset within the row (0‑based) */
    idx_t k = 3 + dpos;                  /* 3 ≤ k ≤ r+2 */
    idx_t g = r + 2;                     /* graph size (n of K_g) */

    seq_t result = binomial(g, k);       /* binomial factor */

    /* multiply by Pochhammer(3, k-3) = 3·4·…·(k-1) = (k-1)! / 2   */
    if (k > 3) {
        seq_t p = 1;
        for (idx_t i = 3; i < k; ++i) {
            p *= (seq_t)i;
        }
        result *= p;
    }
    /* for k==3, Pochhammer product is 1, so result unchanged */

    return result;
}
