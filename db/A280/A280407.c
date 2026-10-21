/*
 * A280407  Number of 2 X 2 matrices with all elements in {-n,..,0,..,n} with
 *          permanent = determinant * n.
 * https://oeis.org/A280407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=-K..K} countProd[n2*k] * countProd[n1*k] where g =
 *            gcd(n-1, n+1), n1 = (n-1)/g, n2 = (n+1)/g, K = floor(n^2 / n2),
 *            and countProd(v) counts ordered pairs (p,q) with p,q∈[-n,n] and
 *            p*q = v. Special cases: a(0)=1, a(1)=45
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count_prod, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/count_prod.h"
#include "../utils/gcd_uint64.h"

/* gcd for unsigned 64-bit integers */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 45;

    /* n >= 2 */
    uint64_t un = (uint64_t)n;
    uint64_t g = gcd_uint64(un - 1, un + 1);      // g = gcd(n-1, n+1)
    uint64_t n1 = (un - 1) / g;
    uint64_t n2 = (un + 1) / g;

    /* K = floor(n^2 / n2) */
    uint64_t n2sq = n1 * n2;   /* n1*n2 = (n-1)(n+1)/g^2 ≤ n^2 */
    uint64_t nn = un;
    uint64_t n_sq = nn * nn;
    uint64_t K = n_sq / n2;    /* n2 divides n^2? We use floor division */

    seq_t total = 0;
    /* loop over k from -K to K inclusive */
    for (int64_t k = -(int64_t)K; k <= (int64_t)K; ++k) {
        int64_t v1 = (int64_t)n2 * k;   /* x = n2*k */
        int64_t v2 = (int64_t)n1 * k;   /* y = n1*k */
        seq_t c1 = count_prod(n, v1);
        seq_t c2 = count_prod(n, v2);
        total += c1 * c2;
    }
    return total;
}
