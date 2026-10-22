/*
 * A353293  Irregular table T(n, k), n > 0, k = 1..A353292(n), read by rows:
 *          the n-th row contains in ascending order the distinct positive
 *          integers k <= n that have at least one common 1-bit with n.
 * https://oeis.org/A353293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   not stated
 * Range:     u8 n <= 7163, u16 n <= 7163, u32 n <= 7163, u64 n <= 7163,
 *            u128 n <= 7163, big any n
 * Helpers:   count_common_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_common_bits.h"
/* Computes OEIS A353293: the n-th term read row‑wise from the irregular
   table T(n,k), where
        T(n,1) = lowest one‑bit of n  (A006519(n))
        T(n,k) = n                  (last entry of the row)
   The row for n contains all integers k (1 ≤ k ≤ n) such that (k & n) != 0
   in ascending order. */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset is 1, but guard against 0 */
    idx_t target = n;
    idx_t cumulative = 0;

    for (idx_t m = 1; ; ++m) {
        /* Count numbers in row m that share a 1‑bit with m */
        idx_t row_len = 0;
        for (idx_t k = 1; k <= m; ++k) {
            if (k & m) ++row_len;
        }

        if (cumulative + row_len >= target) {
            /* n falls in row m. Find the offset‑th element. */
            idx_t offset = target - cumulative;  /* 1‑based within the row */
            idx_t found = 0;
            for (idx_t k = 1; k <= m; ++k) {
                if (k & m) {
                    ++found;
                    if (found == offset) {
                        return (seq_t)k;
                    }
                }
            }
            /* unreachable, but keep compiler happy */
            return (seq_t)0;
        }

        cumulative += row_len;
    }
}
