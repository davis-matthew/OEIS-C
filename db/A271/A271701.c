/*
 * A271701  Triangle read by rows, T(n, k) = Sum_{j=0..n} (-1)^(n-j)*C(-j,
 *          -n)*S2(k, j), S2 the Stirling set numbers A048993, for n >= 0 and
 *          0 <= k <= n.
 * https://oeis.org/A271701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(n,k) = Σ_{j=1..n} binom(n-1, j-1) *
 *            S2(k,j), r = ⌊(√(8n+1)-1)/2⌋; c = n – r(r+1)/2; T(0,0)=1.
 *            S2(k,j) are Stirling numbers of the second kind
 * Range:     u8 n <= 25, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, compute_stirling2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/compute_stirling2.h"

/* Compute row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t triangle_row(idx_t n)
{
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* T(0,0) */

    idx_t r = triangle_row(n);
    unsigned __int128 start_big = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start_big;
    idx_t c = n - start;           /* column index */

    seq_t *S = (seq_t *)calloc((size_t)(c + 1), sizeof(seq_t));
    if (!S) return 0;              /* memory failure fallback */

    compute_stirling2(c, S);

    seq_t sum = 0;
    for (idx_t j = 1; j <= c; ++j) {
        seq_t s2 = S[j];
        if (s2 == 0) continue;
        seq_t bin = binomial(r - 1, j - 1); /* binom(r-1, j-1) */
        sum += bin * s2;
    }

    free(S);
    return sum;
}
