/*
 * A279486  Indices k such that A279484(k) = 0.
 * https://oeis.org/A279486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A279484(k) = 0, where A279484 =
 *            coefficients of Product_{k>=1} (1-x^(k^3)); computed by a signed
 *            subset-sum DP sieve up to 400000 (conjecturally no terms beyond
 *            353684)
 * Range:     u8 n <= 220, u16 n <= 5051, u32 n <= 5216, u64 n <= 5216,
 *            u128 n <= 5216, big any n
 * Validated: standard, pass, 82 terms through n = 5046 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define A279486_LIM 400000

static long long *A279486_terms = 0;
static long long A279486_count = 0;

static void A279486_init(void)
{
    if (A279486_terms) return;
    long long *dp = (long long *)calloc((size_t)A279486_LIM + 1, sizeof(long long));
    A279486_terms = (long long *)calloc((size_t)A279486_LIM + 1, sizeof(long long));
    if (!dp || !A279486_terms) { free(dp); free(A279486_terms); A279486_terms = 0; return; }
    dp[0] = 1;
    for (long long k = 1; k * k * k <= A279486_LIM; ++k) {
        long long c = k * k * k;
        for (long long i = A279486_LIM; i >= c; --i)
            dp[i] -= dp[i - c];
    }
    for (long long i = 1; i <= A279486_LIM; ++i)
        if (dp[i] == 0) A279486_terms[A279486_count++] = i;
    free(dp);
}

seq_t a(idx_t n)
{
    A279486_init();
    if (!A279486_terms || n < 1 || (long long)n > A279486_count) return (seq_t)0;
    return (seq_t)A279486_terms[(long long)n - 1];
}
