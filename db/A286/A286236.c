/*
 * A286236  Square array A(n,k) = P(A000010(k), (n+k-1)/k) if k divides
 *          (n+k-1), 0 otherwise, read by descending antidiagonals as A(1,1),
 *          A(1,2), A(2,1), etc. Here P is a two-argument form of sequence
 *          A000027 used as a pairing function N x N -> N.
 * https://oeis.org/A286236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 9351, u32 n <= 9351, u64 n <= 9351,
 *            u128 n <= 9351, big any n
 * Helpers:   _pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A286236 – Square array A(n,k) = P(A000010(k), (n+k-1)/k)
 *  if k divides (n+k-1), 0 otherwise, read by descending antidiagonals
 *  (A(1,1), A(1,2), A(2,1), A(1,3), …).  P is the Cantor pairing
 *  function with first argument ≥1 and second argument ≥0:
 *      P(a,b) = (a+b)*(a+b+1)/2 – b
 *
 *  For a given index n (1‑based) this function returns the nth term
 *  of the flattened array.
 */

#include <stdbool.h>
#include "../utils/_pair.h"

/* Euler's totient φ(k) for a 64‑bit integer k, returned as seq_t. */
static seq_t _phi(idx_t k)
{
    seq_t result = (seq_t)k;
    idx_t n = k;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, undefined for 0 */

    /* Find diagonal d: d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    idx_t prev = (d - 1) * d / 2;               /* sum of terms before this diagonal */
    idx_t offset = n - prev - 1;                /* 0‑based position inside the diagonal */
    idx_t i = offset + 1;                      /* row index (n in the array) */
    idx_t j = d - offset;                      /* column index (k in the array) */

    /* Divisibility condition: k divides (n + k - 1) */
    if ((i + j - 1) % j != 0) return 0;

    /* Parameters for the pairing function */
    idx_t t = (i + j - 1) / j;                 /* ≥1 */
    seq_t phi_k = _phi(j);                     /* φ(k) */
    return _pair((idx_t)phi_k, t - 1);
}
