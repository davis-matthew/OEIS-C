/*
 * A395974  a(n) = A395975(n, 1), and a(0) = 0.
 * https://oeis.org/A395974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(n,1) with a(0) = 0, where T is the inverse of triangle
 *            A126216 with a left unit column adjoined: T(n,n) = 1, T(n,k) =
 *            -Sum_{j=k+1..n} T(n,j) * C(j-1,k-1)*C(2j-k+1,j-k+1)/(j+1), k < n
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 16, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-10)
 * Helpers:   binomial, gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t         *A395974_memo = NULL;
static unsigned char *A395974_have = NULL;
static idx_t          A395974_cap  = 0;

/* coefficient C(j-1,k-1)*C(2j-k+1,j-k+1)/(j+1), divided exactly */
static seq_t A395974_coef(idx_t j, idx_t k)
{
    seq_t b1 = binomial(j - 1, k - 1);
    seq_t b2 = binomial(2 * j - k + 1, j - k + 1);
    seq_t d  = (seq_t)(j + 1);
    seq_t g  = gcd(b1, d);
    if (g < 0) g = -g;
    if (g == 0) g = 1;
    b1 /= g;
    d  /= g;
    b2 /= d;
    return b1 * b2;
}

static seq_t A395974_compute(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t *row = mem_new(seq_t, (size_t)(n + 1));
    row[n] = (seq_t)1;
    for (idx_t k = n; k-- > 1; ) {
        seq_t sum = (seq_t)0;
        for (idx_t j = k + 1; j <= n; ++j) {
            seq_t term = row[j];
            term *= A395974_coef(j, k);
            sum += term;
        }
        row[k] = -sum;
    }
    seq_t res = row[1];
    mem_free(row);
    return res;
}

seq_t a(idx_t n)
{
    if (n >= A395974_cap) {
        idx_t nc = A395974_cap ? A395974_cap : 16;
        while (nc <= n) nc *= 2;
        seq_t *nm = mem_new(seq_t, (size_t)nc);
        unsigned char *nh = (unsigned char *)calloc((size_t)nc, 1);
        if (!nh) abort();
        for (idx_t i = 0; i < A395974_cap; ++i) {
            nm[i] = A395974_memo[i];
            nh[i] = A395974_have[i];
        }
        if (A395974_memo) mem_free(A395974_memo);
        if (A395974_have) free(A395974_have);
        A395974_memo = nm;
        A395974_have = nh;
        A395974_cap  = nc;
    }
    if (!A395974_have[n]) {
        A395974_memo[n] = A395974_compute(n);
        A395974_have[n] = 1;
    }
    return A395974_memo[n];
}
