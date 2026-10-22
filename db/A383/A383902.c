/*
 * A383902  Square table read by ascending antidiagonals where T(n,k) =
 *          binomial(k+2^n-2,k).
 * https://oeis.org/A383902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(k + 2^n - 2, k) where T(n,k) is read by ascending
 *            antidiagonals. Here n = row index, k = column index, and the
 *            pair (n,k) is extracted from the global index n as described
 *            below
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine diagonal s such that a(n) lies on diagonal s (n + k = s). */
    idx_t s = 0;
    /* The index of the first element of diagonal s is s*(s+1)/2. */
    while ((s + 1) * (s + 2) / 2 <= n) {
        s++;
    }

    /* offset within the diagonal: 0 <= offset <= s. */
    idx_t offset = n - s * (s + 1) / 2;

    /* The diagonal is traversed from (n = s, k = 0) down to (n = 0, k = s). */
    idx_t n_val = s - offset;  /* row index */
    idx_t k_val = offset;      /* column index */

    /* Handle trivial cases without needing to evaluate 2^n. */
    if (k_val == 0) {
        return (seq_t)1;
    }
    if (n_val == 0) {
        return (seq_t)0;  /* k_val > 0 at this point */
    }

    /* 2^n_val as a big integer. */
    seq_t two_n = pow2(n_val);

    /* N = k + 2^n - 2 */
    seq_t N = two_n - (seq_t)2 + (seq_t)k_val;

    /* Compute binomial(N, k_val) multiplicatively. */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k_val; ++i) {
        /* numerator = N - (k_val - i) */
        seq_t numerator = N - (seq_t)(k_val - i);
        result = result * numerator / (seq_t)i;
    }
    return result;
}
