/*
 * A286234  Square array A(n,k) = P(A000010(k), floor((n+k-1)/k)), read by
 *          descending antidiagonals as A(1,1), A(1,2), A(2,1), etc. Here P is
 *          a two-argument form of sequence A000027 used as a pairing function
 *          N x N -> N.
 * https://oeis.org/A286234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A286234: square array A(n,k) = P(φ(k), floor((n+k-1)/k)) where φ
 *            is Euler's totient (A000010), P is the Cantor pairing function
 *            for 1‑based indices: P(a,b) = (a+b-1)*(a+b-2)/2 + a. The array
 *            is read by descending antidiagonals: A(1,1), A(1,2), A(2,1),
 *            A(1,3), A(2,2), A(3,1), … This implementation...
 * Range:     u8 n <= 299, u16 n <= 9612, u32 n <= 9612, u64 n <= 9612,
 *            u128 n <= 9612, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euler's totient function for 64‑bit unsigned integers. */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp   = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* Main routine for OEIS A286234. */
seq_t a(idx_t n)
{
    /* Locate the coordinates (i,k) corresponding to the n‑th element
       in descending‑antidiagonal ordering.  We compute the diagonal
       sum s (i + k) using the triangular-number identity.
       The number of terms up to diagonal s is T_s = (s‑1)*s/2. */
    idx_t s = 2;
    /* Find the smallest s such that T_s >= n.  For n ≤ 10^4
       this loop runs at most about 150 iterations. */
    while ((s - 1) * s / 2 < n) ++s;

    /* Previous diagonal count: T_{s‑1} = (s‑2)*(s‑1)/2. */
    idx_t prev = (s - 2) * (s - 1) / 2;
    /* Offset k within the s‑th diagonal (0‑based). */
    idx_t offset = n - prev - 1;

    /* Coordinates on the diagonal: i starts at 1, j decreases. */
    idx_t i = 1 + offset;   /* row index */
    idx_t k = s - 1 - offset; /* column index (k in A(n,k)) */

    /* Compute φ(k). */
    uint64_t phi_k = phi_uint64((uint64_t)k);

    /* Compute floor((i + k - 1) / k). */
    idx_t t = (i + k - 1) / k;

    /* Cantor pairing P(φ_k, t).  All arithmetic occurs in seq_t
       to avoid truncation. */
    seq_t phi  = (seq_t)phi_k;
    seq_t tt   = (seq_t)t;
    seq_t sum  = phi + tt;
    seq_t pair = ((sum - 1) * (sum - 2)) / 2 + phi;
    return pair;
}
