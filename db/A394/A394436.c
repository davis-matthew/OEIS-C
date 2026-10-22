/*
 * A394436  Triangle read by rows: Bell-transform of the number of
 *          endofunctions on [n].
 * https://oeis.org/A394436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let idxAny be the input index n. Find the unique integers r, k
 *            with r*(r+1)/2 <= idxAny < (r+1)*(r+2)/2 k = idxAny - r*(r+1)/2
 *            (0 ≤ k ≤ r) Then a(idxAny) = T(r,k), where T(n,k) =
 *            binomial(n,k) * Σ_{j=0}^{n-k} binomial(n-k,j) * rf(k-1,j) *
 *            n^{\,n-k-j}, rf(a,b) = Π_{t=0}^{b-1} (a+t) (rising factori...
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below follows exactly this definition.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n_input)
{
    /* Handle the trivial case immediately */
    if (n_input == 0) return (seq_t)1;

    /* --------- Find row r and column k from index n_input --------- */
    /* We need r such that r*(r+1)/2 <= n_input < (r+1)*(r+2)/2. */
    /* Use an integer binary search on r. */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n_input) high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* mid is biased up */
        if (mid * (mid + 1) / 2 <= n_input)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t k = n_input - r * (r + 1) / 2;

    /* --------- Compute T(r,k) ------------------------------------ */
    /* Special case k == 0.  For n>0 the value is zero. */
    if (k == 0) return (seq_t)(r == 0 ? 1 : 0);

    /* Prepare common parameters */
    idx_t nk = r - k;          /* n-k   */
    seq_t power_n = pow_seq((seq_t)r, nk);  /* n^{n-k}  */
    seq_t rf = (seq_t)1;               /* rf(k-1, 0) = 1 */
    seq_t sum  = (seq_t)0;

    for (idx_t j = 0; j <= nk; ++j) {
        /* binomial(n-k, j) */
        seq_t bin_rk_j = binomial(nk, j);

        /* Multiply by rf(k-1, j) */
        if (j > 0)
            rf *= (seq_t)(k - 1 + j - 1);   /* multiply by (k-1 + (j-1)) */

        /* term = bin * rf * power_n */
        seq_t term = bin_rk_j * rf * power_n;
        sum += term;

        /* Prepare power_n for next j: power_n /= n */
        if (j < nk)
            power_n /= (seq_t)r;   /* exact division */
    }

    /* Final multiplication by binomial(r, k) */
    seq_t bin_r_k = binomial(r, k);
    return bin_r_k * sum;
}
