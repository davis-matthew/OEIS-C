/*
 * A376944  G.f.: Sum_{k>=0} 2^k * x^(k*(k+1)/2) * Product_{j=1..k} (1 + x^j).
 * https://oeis.org/A376944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 76, u32 n <= 251, u64 n <= 883,
 *            u128 n <= 3257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376944
   Generating function: Sum_{k>=0} 2^k x^{k(k+1)/2} Prod_{j=1..k} (1 + x^j)
   Computes a(n) = Σ_{k: k(k+1)/2 ≤ n} 2^k * #{S ⊆ {1..k} : ΣS = n - k(k+1)/2}
   via dynamic programming over subsets.
*/
#include <stdlib.h>
#include <stddef.h>

/* Compute term a(n) of A376944.
   idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* allow trivial case with no allocation                       */
    if (n == 0)
        return (seq_t)1;

    /* maximum sum needed is n, allocate DP array accordingly    */
    size_t N = (size_t)n;                 /* safe cast for array size */
    seq_t *f = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!f) return (seq_t)0;             /* allocation failure fallback */

    /* DP initialization: empty set */
    f[0] = (seq_t)1;

    seq_t result = (seq_t)0;
    seq_t pow2   = (seq_t)1;             /* 2^0 */

    /* Contribution from k = 0 (triangular number 0)                */
    /* f[n] is already 0 for n > 0, remains 0 for n = 0             */
    result = result + pow2 * f[(size_t)n];

    /* Iterate over possible k, updating DP and accumulating result */
    for (idx_t k = 1;; ++k)
    {
        /* triangular number k(k+1)/2                                     */
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (tri > n)
            break;  /* no further k can contribute for this n */

        /* multiply by 2 to get 2^k                                      */
        pow2 = pow2 * (seq_t)2;

        /* add element k to all subsets – standard 0/1 knapsack trick */
        for (size_t s = N; s >= (size_t)k; --s)
        {
            f[s] = f[s] + f[s - (size_t)k];
        }

        /* remaining sum after subtracting the triangular part         */
        idx_t rem = n - (idx_t)tri;   /* guaranteed to fit in idx_t */
        if (rem <= N)
            result = result + pow2 * f[(size_t)rem];
    }

    free(f);
    return result;
}
