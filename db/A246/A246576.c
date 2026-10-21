/*
 * A246576  G.f.: (Product_{r>=1} (1 - x^r))*x^(k^2)/Product_{i=1..k} ((1 -
 *          x^i)^2) with k=2.
 * https://oeis.org/A246576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G(x) = (∏_{r≥1} (1 - x^r)) * x^4 / ((1 - x)^2 * (1 - x^2)^2)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of x^n in G(x).  a(0)=0. */

#include <stdlib.h>
#include <stdint.h>

/* Cached table of coefficients of B(x) = 1/((1-x)^2(1-x^2)^2) */
static idx_t _b_cache_max = 0;
static seq_t * _b_cache = NULL;

/* Ensure _b_cache contains values b[0..N]  (b[i] = coefficient of x^i in B(x)). */
static void ensure_b(idx_t N)
{
    if (_b_cache == NULL) {
        /* Allocate table up to N */
        _b_cache = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
        if (_b_cache == NULL) return; /* allocation failure left to caller */
        for (idx_t n = 0; n <= N; ++n) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= n / 2; ++j) {
                sum += (seq_t)(n - 2 * j + 1) * (j + 1);
            }
            _b_cache[n] = sum;
        }
        _b_cache_max = N;
    } else if (N > _b_cache_max) {
        /* Extend existing table */
        seq_t *tmp = (seq_t *)realloc(_b_cache, (size_t)(N + 1) * sizeof(seq_t));
        if (tmp == NULL) return; /* allocation failure left to caller */
        _b_cache = tmp;
        for (idx_t n = _b_cache_max + 1; n <= N; ++n) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= n / 2; ++j) {
                sum += (seq_t)(n - 2 * j + 1) * (j + 1);
            }
            _b_cache[n] = sum;
        }
        _b_cache_max = N;
    }
}

/* Return a(n) for the OEIS sequence A246576. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;
    idx_t N = n - 4;         /* coefficient required is for x^(N) in P * B */
    ensure_b(N);
    seq_t result = 0;

    /* k = 0 term (g(0) = 0, sign = +1) */
    result += _b_cache[N];

    /* k >= 1 generalized pentagonal terms */
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        if (g1 > N) break;
        seq_t sign = (k % 2 == 0) ? 1 : -1;        /* (-1)^k */
        result += sign * _b_cache[N - g1];

        idx_t g2 = k * (3 * k + 1) / 2;
        if (g2 <= N) {
            result += sign * _b_cache[N - g2];
        }
    }

    return result;
}
