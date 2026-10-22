/*
 * A346148  Square array read by descending antidiagonals: T(n, k) = mu^n(k)
 *          where mu^1(k) = mu(k) = A008683(k) and for each n >= 1,
 *          mu^(n+1)(k) is the Dirichlet convolution of mu(k) and mu^n(k).
 * https://oeis.org/A346148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth term of the square array T(r,c)=μⁿ(c) read by
 *            descending antidiagonals. Each element is μⁿ(c) where μ is the
 *            Möbius function, and μⁿ is the n‑fold Dirichlet convolution of μ
 *            with itself. For c = ∏pⱼ^mⱼ one has μⁿ(c) = ∏ binomial(n, mⱼ) *
 *            (-1)^mⱼ
 * Range:     i8 n <= 147, i16 n <= 1011, i32 n <= 9870, i64 n <= 9870,
 *            i128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small binomial used for the exponents (exponent ≤ log₂(c) ≤ 64) */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i);
        r = r / i;
    }
    return r;
}

/* μⁿ(c) for given row n and col c.  c≥1, n≥1 */
static seq_t mu_power(idx_t n, idx_t c)
{
    seq_t res = 1;
    idx_t tmp = c;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        idx_t exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        seq_t b = binom_small(n, exp);
        res *= b;
        if (exp & 1) res = -res;
    }

    /* odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t b = binom_small(n, exp);
            res *= b;
            if (exp & 1) res = -res;
        }
    }

    /* remaining prime factor (exponent 1) */
    if (tmp > 1) {
        seq_t b = binom_small(n, 1); /* = n */
        res *= b;
        res = -res;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find antidiagonal d such that d(d−1)/2 < n ≤ d(d+1)/2   */
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++d;
        sum += d;
    }

    /* index inside this antidiagonal (0‑based) */
    idx_t prev_sum = d * (d - 1) / 2;   /* integer division */
    idx_t t = n - prev_sum - 1;       /* 0 ≤ t < d */

    idx_t r = t + 1;
    idx_t c = d - t;

    return mu_power(r, c);
}
