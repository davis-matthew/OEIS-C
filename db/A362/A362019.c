/*
 * A362019  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = (-1)^n * Sum_{j=0..n} (-k*j)^j *
 *          binomial(n,j).
 * https://oeis.org/A362019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) = (-1)^n *
 *            Σ_{j=0..n} (-k*j)^j * binomial(n,j) along its antidiagonals
 *            downwards: for each diagonal d the entries are (n=0,k=d),
 *            (n=1,k=d-1), … , (n=d,k=0)
 * Range:     i8 n <= 17, i16 n <= 32, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions are provided by the utils library. */
#include "../utils/seq_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

/* Internal helper: compute one array entry T(n,k). */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t bin  = binomial(n, j);                     /* C(n,j)  */
        seq_t base = -((seq_t)k * (seq_t)j);              /* -(k*j)   */
        seq_t pow_term = seq_pow(base, j);                /* base^j   */
        seq_t prod = seq_mul(pow_term, bin);              /* prod = base^j * C(n,j) */
        sum += prod;
    }
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)(1);
    return sum * sign;                                   /* (-1)^n * sum */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the diagonal number 'd' such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while (((__uint128_t)(d + 1) * (d + 2) / 2) <= n) ++d;

    /* The first index of diagonal d is d(d+1)/2. */
    idx_t start = d * (d + 1) / 2;

    /* Position within the diagonal: i rows from the top. */
    idx_t i = n - start;          /* row index (0‑based) */
    idx_t j = d - i;              /* column index (0‑based) */

    return T_value(i, j);
}
