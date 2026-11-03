/*
 * A368235  Triangle read by rows: n-th row polynomial equals the numerator of
 *          the rational function (-1)^n*f(x) * (d/dx)^n (1/f(x)), where f(x)
 *          = sqrt(x + x^2).
 * https://oeis.org/A368235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: a(n) is the n‑th term (row‑major) of the triangle
 * T(r,c) defined by
 *   T(r,c) = r! * 2^(2c−r) * binomial(r,c) * binomial(2r−2c, r−c)
 * for r,c≥0.
 * The sequence begins 1,1,2,3,8,8,15,54,72,48,105,480,864,768,384,945,...
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t _factorial(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i)
        res *= (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* --- locate row r and column c of the n-th element (0‑based) --- */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;              /* after loop: (r+1)(r+2)/2 > n  and r*(r+1)/2 ≤ n */
    idx_t prev_sum = r * (r + 1) / 2;   /* number of elements before row r */
    idx_t c = n - prev_sum;            /* column index within row r */

    /* --- compute T(r,c) using the integer formula --- */
    seq_t fact_r   = _factorial(r);
    seq_t fact_c   = _factorial(c);
    seq_t fact_rc  = _factorial(r - c);

    seq_t t = fact_r * fact_r;          /* (r!)² */
    t /= fact_c;                       /* divide by c! */
    t /= fact_rc;                      /* divide by (r‑c)! */

    seq_t binom_2rc = binomial(2 * r - 2 * c, r - c);
    t *= binom_2rc;                    /* multiply by binom(2r−2c, r−c) */

    /* apply the power of two factor 2^(2c−r) */
    int exp = 2 * (int)c - (int)r;
    if (exp >= 0)
        t *= pow2((idx_t)exp);
    else
        t /= pow2((idx_t)(-exp));

    return t;
}
