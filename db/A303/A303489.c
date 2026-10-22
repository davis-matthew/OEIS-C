/*
 * A303489  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = n! * [x^n] 1/(1 - k*x)^(n/k).
 * https://oeis.org/A303489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = Product_{j=0..n-1} (k*j + n), a(n) is the index of this
 *            square array read by antidiagonals (n>=0, k>=1)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t N) {
    /* Find the diagonal d such that d*(d+1)/2 > N. */
    idx_t low = 0;
    idx_t high = N + 1;  /* high is always larger than any needed diagonal */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri > N)
            high = mid;
        else
            low = mid;
    }
    idx_t d = high;

    /* Position inside diagonal */
    unsigned __int128 t_prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t pos = (idx_t)(N - t_prev);

    /* Coordinates (n,k) of the term at index N */
    idx_t n_val = pos;      /* n runs from 0 upward on each diagonal */
    idx_t k = d - pos;      /* k = d - n */

    /* Compute A(n_val,k) = product_{j=0..n_val-1} (k*j + n_val) */
    seq_t result = 1;
    for (idx_t j = 0; j < n_val; ++j) {
        result *= (seq_t)(k * j + n_val);
    }
    return result;
}
