/*
 * A241500  Triangle T(n,k): number of ways of partitioning the n-element
 *          multiset {1,1,2,3,...,n-1} into exactly k nonempty parts, n>=1 and
 *          1<=k<=n.
 * https://oeis.org/A241500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th element of the triangle * T(n,k) = S(n‑1,k) +
 *            S(n‑1,k‑1) + C(k,2)*S(n‑2,k), where S * is the Stirling number
 *            of the second kind. The triangle is * flattened row‑by‑row:
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), * a(4)=T(3,1), …
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global cache for Stirling numbers */
static seq_t *S = NULL;           /* pointer to 2‑D array stored linearly */
static size_t S_sz = 0;           /* dimension of the square array */
static size_t S_max = 0;          /* largest n for which we have S(n,·) */

/* Access a Stirling number, returning 0 if indices are out of range */
static inline seq_t getS(size_t n, size_t k)
{
    if (n > S_max || k > S_max || k > n)
        return 0;
    return S[n * S_sz + k];
}

/* Rebuild the Stirling table up to a given n (inclusive). */
static void ensureS(size_t up_to)
{
    if (up_to <= S_max)
        return;

    size_t new_sz = up_to + 1;          /* we need indices 0…up_to */
    seq_t *new_S = (seq_t *)malloc(new_sz * new_sz * sizeof(seq_t));
    if (!new_S) /* allocation failed */
        return;

    /* initialise to zero */
    for (size_t i = 0; i < new_sz * new_sz; ++i)
        new_S[i] = 0;

    /* S(0,0) = 1 */
    new_S[0 * new_sz + 0] = 1;

    for (size_t i = 1; i < new_sz; ++i) {
        for (size_t k = 1; k <= i; ++k) {
            seq_t part1 = new_S[(i - 1) * new_sz + (k - 1)];
            seq_t part2 = new_S[(i - 1) * new_sz + k] * (seq_t)k;
            new_S[i * new_sz + k] = part1 + part2;
        }
    }

    free(S);
    S = new_S;
    S_sz = new_sz;
    S_max = up_to;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0 */

    /* Determine the row r such that r*(r+1)/2 >= n */
    size_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Number of elements before this row */
    size_t prev_sum = (r - 1) * r / 2;

    size_t col = (size_t)n - prev_sum;      /* 1-based column index */

    /* Ensure we have enough Stirling numbers computed */
    ensureS(r);

    /* Compute T(r, col) according to the formula */
    seq_t term1 = 0;     /* S(r-1, col) */
    if (col <= r - 1)
        term1 = getS(r - 1, col);

    seq_t term2 = getS(r - 1, col - 1);     /* always valid */

    seq_t term3 = 0;     /* binom(col,2) * S(r-2, col) */
    if (col >= 2 && r >= 2) {
        seq_t binom = ((seq_t)col * (seq_t)(col - 1)) / 2;
        seq_t s_val = getS(r - 2, col);
        term3 = binom * s_val;
    }

    return term1 + term2 + term3;
}
