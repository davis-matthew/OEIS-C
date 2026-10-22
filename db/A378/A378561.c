/*
 * A378561  Number of ways to place k nonattacking anassas on an n X n chess
 *          board. Triangle T(n,k) read by rows.
 * https://oeis.org/A378561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) of * number of
 *            ways to place k nonattacking anassas on an n×n board, * where
 *            T(n,k) = Σ_{j=0..⌈k/2⌉} j! * C(n-k+j,j) * S(n,n-k+j) * *
 *            2^{k-2j} * (C(k-j,j-1)+C(k-j+1,j))
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Integer square root of a 64‑bit unsigned integer */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32) - 1, ans = 0;
    while (lo <= hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Small factorial routine (values are small – at most ~140) */
static seq_t small_factorial(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i) res *= i;
    return res;
}

/* Compute the n-th term of the sequence (flattened triangle) */
seq_t a(idx_t n)
{
    /* Determine the triangle row (n) and position (k) */
    uint64_t val  = (uint64_t)n * 8ULL + 1ULL;          /* 8*n + 1  */
    uint64_t sqrt_val = isqrt_u64(val);                /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((sqrt_val - 1ULL) / 2ULL);        /* row number */
    idx_t row_start = r * (r + 1) / 2;                 /* first index of this row */
    idx_t k = n - row_start;                           /* column within the row */

    /* Compute Stirling numbers of the second kind S(i,j) for i = 0..r */
    seq_t *Srow = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!Srow) return 0;   /* memory failure – will not be reached in test harness */
    Srow[0] = 1;           /* S(0,0) = 1 */
    for (idx_t i = 1; i <= r; ++i)
    {
        /* Update in reverse order to avoid extra array */
        for (idx_t k2 = i; k2 >= 1; --k2)
            Srow[k2] = (seq_t)k2 * Srow[k2] + Srow[k2 - 1];
        Srow[0] = 0;   /* S(i,0) = 0 for i > 0 */
    }

    /* Main summation over j */
    seq_t result = 0;
    idx_t jmax = (k + 1) / 2;   /* ceil(k/2) */
    for (idx_t j = 0; j <= jmax; ++j)
    {
        seq_t term = 0;
        seq_t fact = small_factorial(j);
        seq_t bin1 = binomial(r - k + j, j);
        seq_t Sval = Srow[r - k + j];

        /* Compute factorA and factorB */
        seq_t factorA = 0;
        if (j > 0)
            factorA = binomial(k - j, j - 1);
        seq_t factorB = binomial(k - j + 1, j);
        seq_t factor = factorA + factorB;

        /* Product without the power of two */
        seq_t num = fact * bin1;
        num = num * Sval;
        num = num * factor;

        /* Handle the power of two, including negative exponents */
        int exp = (int)(k - 2 * j);
        if (exp >= 0)
        {
            seq_t pow2e = pow2((idx_t)exp);
            term = num * pow2e;
        }
        else
        {
            seq_t pow2e = pow2((idx_t)(-exp));
            term = num / pow2e;   /* division is exact by definition of the formula */
        }

        result += term;
    }

    free(Srow);
    return result;
}
