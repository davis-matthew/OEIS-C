/*
 * A368065  a(n) = Product_{i=1..n, j=1..n} (i^2 + 5*i*j + j^2).
 * https://oeis.org/A368065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n, j=1..n} (i^2 + 5*i*j + j^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t si2 = si * si;

        for (idx_t j = i; j <= n; j++) {
            seq_t sj = (seq_t)j;
            seq_t sj2 = sj * sj;

            /* val = i^2 + 5*i*j + j^2 */
            seq_t val = si2 + ((seq_t)5) * si * sj + sj2;

            result *= val;
            if (i != j) result *= val;   /* double for off‑diagonal terms */
        }
    }
    return result;
}
