/*
 * A244121  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n^n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i(i+1)/2 ≤ n < (i+1)(i+2)/2 * and j = n −
 *            i(i+1)/2. * The triangle entry is * T(n,k) =
 *            n*(n-k)^(k-1)*k^(n-k)*binomial(n,k) for k>0, * T(n,0) = 0^n
 *            (conventionally 1 when n=0, 0 otherwise). * This sequence lists
 *            the triangle entries row‑by‑row
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 159,
 *            u128 n <= 416, big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute row index i */
    if (n == 0) {
        return 1; /* T(0,0) = 1 */
    }
    double root = sqrt((double)(8 * (long double)n + 1));
    idx_t i = (idx_t)((root - 1) * 0.5);

    /* Adjust i to satisfy i(i+1)/2 <= n */
    while (i * (i + 1) / 2 > n) i--;
    while ((i + 1) * (i + 2) / 2 <= n) i++;

    idx_t j = n - i * (i + 1) / 2; /* column index */

    /* Handle the zero column special case */
    if (j == 0) {
        return (i == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* j>0: compute the binomial decomposition entry */
    idx_t k = j;
    seq_t bin = binomial((idx_t)i, (idx_t)k);
    seq_t pow1 = seq_pow((seq_t)(i - k), (idx_t)(k - 1)); /* (i-k)^(k-1) */
    seq_t pow2 = seq_pow((seq_t)k, (idx_t)(i - k));     /* k^(i-k) */

    seq_t res = bin;
    res = res * pow2;
    res = res * pow1;
    res = res * (seq_t)i; /* multiply by n (which is i) */
    return res;
}
