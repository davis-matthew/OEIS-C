/*
 * A279310  Numerators of coefficients in expansion of (cos x)/(1 - x - x^2).
 * https://oeis.org/A279310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   f(x)=cos x/(1-x-x^2)=∑_{n≥0} c_n x^n * with c_0=1, c_1=1 and
 *            recursion * c_n = c_{n-1} + c_{n-2} + d_n, * where d_n = 0 if n
 *            is odd and d_n = (-1)^{n/2}/n! if n is even. * a(n) is the
 *            numerator of c_n in lowest terms, i.e. * a(n) = p_n /
 *            gcd(p_n,D_n), D_n = n! (n even) or (n-1)! (n odd)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *raw_num = NULL;   /* raw (un‑reduced) numerators */
static seq_t *ans_num = NULL;   /* reduced numerators = a(n) */
static size_t cap = 0;          /* allocated capacity of the arrays */
static idx_t  len = 0;          /* number of terms already computed (next index = len) */
static seq_t  fact = 1;         /* factorial of the previous index (i after processing i) */

/* Ensure that terms up to index n are computed. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= len)
        return;   /* already computed */

    /* Initial allocation */
    if (len == 0) {
        cap = 4;
        raw_num = (seq_t *)malloc(cap * sizeof(seq_t));
        ans_num = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!raw_num || !ans_num) abort();

        raw_num[0] = (seq_t)1;  ans_num[0] = (seq_t)1;  /* c_0 = 1/1 */
        raw_num[1] = (seq_t)1;  ans_num[1] = (seq_t)1;  /* c_1 = 1/1 */

        fact = (seq_t)1;             /* 1! after i = 1 */
        len  = 2;                    /* indices 0 and 1 initialized */
    }

    /* Grow capacity if needed */
    if ((size_t)(n + 1) > cap) {
        while ((size_t)(n + 1) > cap) cap <<= 1;
        raw_num = (seq_t *)realloc(raw_num, cap * sizeof(seq_t));
        ans_num = (seq_t *)realloc(ans_num, cap * sizeof(seq_t));
        if (!raw_num || !ans_num) abort();
    }

    /* Compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        seq_t raw;

        if (i == 0 || i == 1) continue;   /* already set */

        if ((i & 1) == 0) {               /* i even: include d_i */
            seq_t sum = raw_num[i-1] + raw_num[i-2];
            sum *= (seq_t)i * (seq_t)(i-1);
            /* (-1)^{i/2} is +1 for even i/2, -1 otherwise */
            if (((i >> 1) & 1) == 0)
                sum += (seq_t)1;
            else
                sum -= (seq_t)1;
            raw = sum;
        } else {                          /* i odd: d_i = 0 */
            raw = raw_num[i-1] + raw_num[i-2] * (seq_t)(i-1) * (seq_t)(i-2);
        }

        /* Update factorial to i! */
        fact *= (seq_t)i;

        /* Denominator for c_i */
        seq_t D = ((i & 1) == 0) ? fact : fact / (seq_t)i;

        /* Reduce numerator and store the answer */
        seq_t g = gcd(raw, D);
        ans_num[i] = raw / g;
        raw_num[i] = raw;   /* keep raw for future calculations */
    }

    len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return ans_num[n];
}
