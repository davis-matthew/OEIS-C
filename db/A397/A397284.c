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
 * Validated: standard, pass, 61 terms through n = 9801 (2026-11-01)
 * Helpers:   binomial, ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* auxiliary: determine row r and triangular offset for given n */
static inline void row_and_index(idx_t n, idx_t *r, idx_t *k)
{
    idx_t t = 0;      /* current triangular number t = r(r+1)/2 */
    idx_t rr = 0;     /* current r */
    /* find the largest r with t <= n */
    while (t + rr + 1 <= n) {
        ++rr;
        t += rr;      /* now t = rr(rr+1)/2 */
    }
    *r = rr;
    *k = n - t;
}

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
    seq_t pow_val = ipow(base, exp);

    /* 2^k */
    seq_t two_pow_k = pow2(k);

    /* multiply all parts together */
    seq_t tmp = seq_mul(bin, pow_val);
    return seq_mul(tmp, two_pow_k);
}
