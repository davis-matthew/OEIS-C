/*
 * A397284  Triangle read by rows: T(n, k) = 2^n * P_{n, k}(1/2) where P_{n,
 *          k}(x) = x * (n - k + x)^(n - k - 1) * binomial(n, k).
 * https://oeis.org/A397284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle read by rows T(r,k) = 2^k
 *            · ( 2(r−k)+1 )^(r−k−1) · binomial(r,k), where r is the unique
 *            integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n − r(r+1)/2.
 *            For k = r the exponent is −1 and T(r,r) = 2^r
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 119, u128 n <= 299,
 *            big any n
 * Helpers:   big_pow, binomial, pow2, row_and_index, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/pow2.h"
#include "../utils/big_pow.h"
#include "../utils/row_and_index.h"

seq_t a(idx_t n)
{
    idx_t r, k;
    row_and_index(n, &r, &k);

    /* special case k == r  →  T(r,r) = 2^r */
    if (k == r)
        return pow2(r);

    /* binomial(n,k) */
    seq_t bin = binomial(r, k);

    /* compute base = 2(r-k)+1  and exp = r-k-1 */
    seq_t base = (seq_t)((2 * r) - 2 * k + 1);
    seq_t exp  = (seq_t)(r - k - 1);

    /* base^exp */
    seq_t pow_val = big_pow(base, exp);

    /* 2^k */
    seq_t two_pow_k = pow2(k);

    /* multiply all parts together */
    seq_t tmp = seq_mul(bin, pow_val);
    return seq_mul(tmp, two_pow_k);
}
