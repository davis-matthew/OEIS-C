/*
 * A302542  Expansion of e.g.f. arctan(x)/cos(x) (odd powers only).
 * https://oeis.org/A302542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n+1)! * [x^(2*n+1)] arctan(x)/cos(x)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

static seq_t *euler = NULL;       /* euler[m] = Euler zig number E_{2m} */
static idx_t euler_size = 0;

/* Ensure Euler zig numbers up to m are available */
static void ensure_euler(idx_t m)
{
    if (euler_size > m) return;

    idx_t new_size = euler_size ? euler_size * 2 : 1;
    while (new_size <= m) new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(euler, new_size * sizeof(seq_t));
    if (!tmp) abort();
    euler = tmp;

    for (idx_t k = euler_size; k <= m; ++k) {
        if (k == 0) {
            euler[0] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t i = 1; i <= k; ++i) {
                seq_t term = binom(2 * k, 2 * i);
                term = term * euler[k - i];
                if ( (i + 1) & 1 ) term = -term;
                sum += term;
            }
            euler[k] = sum;
        }
    }
    euler_size = m + 1;
}

/* Compute a single term a(n) with all involved arrays ready */
static seq_t compute_a(idx_t n)
{
    ensure_euler(n);

    seq_t sum = 0;
    seq_t fact_2k = 1;                      /* (2k)!  */
    seq_t binom = (seq_t)(2 * n + 1);        /* C(2n+1, 1) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = euler[n - k];
        term = term * binom;
        term = term * fact_2k;
        if (k & 1) term = -term;
        sum += term;

        if (k == n) break;

        /* update factorial for next k: (2(k+1))! = (2k)! * (2k+1) * (2k+2) */
        fact_2k *= (seq_t)(2 * k + 1);
        fact_2k *= (seq_t)(2 * k + 2);

        /* update binomial: C(2n+1, 2k+3) from C(2n+1, 2k+1) */
        seq_t mult1 = (seq_t)(2 * n - 2 * k);          /* N - (2k+1) */
        seq_t mult2 = (seq_t)(2 * n - 2 * k - 1);      /* N - (2k+2) */
        seq_t den1  = (seq_t)(2 * k + 2);
        seq_t den2  = (seq_t)(2 * k + 3);

        seq_t denom = den1;
        denom *= den2;                               /* den1 * den2 */
        binom *= mult1;
        binom *= mult2;
        binom /= denom;                              /* exact division */
    }

    return sum;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;
        a_cache_size = 1;
    }

    if (n >= a_cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        for (idx_t i = a_cache_size; i <= n; ++i) {
            a_cache[i] = compute_a(i);
        }
        a_cache_size = new_size;
    }
    return a_cache[n];
}
