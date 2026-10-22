/*
 * A305402  A number triangle T(n,k) read by rows for 0<=k<=n, related to the
 *          Taylor expansion of f(u, p) =
 *          (1/2)*(1+1/(sqrt(1-u^2)))*exp(p*sqrt(1-u^2)).
 * https://oeis.org/A305402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 27, i32 n <= 65, i64 n <= 170, i128 n <= 434,
 *            big any n
 * Helpers:   floor_sqrt, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305402: row‑wise flattening of the triangle
 *  T(n,k) = (-1)^k * 2^(k-n+1) * n * (2*n-k-1)! / (k! * (n-k)!)
 *  This function returns a(n) = T(i,j) where i(j) are the row and column
 *  indices in the triangle.
 *
 *  The implementation uses prime factorisation with cancellation to
 *  avoid overflow of intermediate values, and works for any seq_t
 *  signed integer type (fixed or arbitrary precision).
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_seq.h"

/* maximal value needed for the prime sieve (n <= 10000 ⇒ 2n‑k‑1 <= 20000) */
#define MAX_SPF 20000

/* smallest prime factor table */
static int spf[MAX_SPF + 1];
static bool spf_ready = false;

static void init_spf(void)
{
    for (int i = 0; i <= MAX_SPF; ++i) spf[i] = i;
    for (int i = 2; (long long)i * i <= MAX_SPF; ++i)
        if (spf[i] == i)
            for (int j = i * i; j <= MAX_SPF; j += i)
                if (spf[j] == j) spf[j] = i;
    spf_ready = true;
}

/* add prime factor exponents of x to array exp (sign = +1 = add, -1 = subtract) */
static void factorise(idx_t x, int sign, int exp[])
{
    while (x > 1)
    {
        int p = spf[(int)x];
        int cnt = 0;
        while (x % p == 0)
        {
            x /= p;
            ++cnt;
        }
        exp[p] += sign * cnt;
    }
}

/* compute T(n,k) */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;

    /* initialise prime table once */
    if (!spf_ready) init_spf();

    /* exponent array for all primes up to MAX_SPF */
    int exp[MAX_SPF + 1] = {0};

    /* exponent of 2 for the power of two factor */
    int exp2 = (int)k - (int)n + 1;
    exp[2] += exp2;

    /* factorise the component 'n' */
    factorise(n, +1, exp);

    /* numerator factorial (2n-k-1)! */
    idx_t numer = 2 * n - k - 1;
    for (idx_t t = 1; t <= numer; ++t) factorise(t, +1, exp);

    /* denominator factorial k! */
    for (idx_t t = 1; t <= k; ++t) factorise(t, -1, exp);

    /* denominator factorial (n-k)! */
    idx_t denom2 = n - k;
    for (idx_t t = 1; t <= denom2; ++t) factorise(t, -1, exp);

    /* reconstruct the integer from prime exponents */
    seq_t result = (seq_t)1;
    for (int p = 2; p <= MAX_SPF; ++p)
    {
        int e = exp[p];
        if (e == 0) continue;
        if (e > 0)
            result *= pow_seq((seq_t)p, e);
        else
            result /= pow_seq((seq_t)p, -e);
    }

    /* apply sign factor (-1)^k */
    if (k & 1) result = -result;

    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find row i such that i(i+1)/2 <= n < (i+1)(i+2)/2 */
    seq_t sq = floor_sqrt((seq_t)8 * (seq_t)n + 1);
    idx_t i = (idx_t)((sq - 1) / 2);
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;     /* column within the row */

    return compute_T(i, j);
}
