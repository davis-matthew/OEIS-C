/*
 * A333529  Number of triples [n,k,m] with n <= k <= m satisfying T_n + T_k =
 *          T_m, where T_i = i*(i+1)/2 are the triangular numbers.
 * https://oeis.org/A333529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of triples [n,k,m] with n <= k <= m such that T_n
 *            + T_k = T_m, where T_i = i*(i+1)/2. Computation is via divisor
 *            enumeration of n(n+1)
 * Range:     u8 n <= 15, u16 n <= 289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset: a(1) corresponds to n=1 */

    seq_t P = (seq_t)n * ((seq_t)n + 1);   /* n*(n+1) */

    /* maximum number of divisors for n(n+1) with n<=10000 is below 2000 */
    seq_t divs[20000];
    size_t nd = 0;

    for (idx_t d = 1; (seq_t)d * (seq_t)d <= P; ++d) {
        if (P % d == 0) {
            divs[nd++] = d;
            seq_t f = P / d;
            if (f != d) divs[nd++] = f;
        }
    }

    seq_t count = 0;
    seq_t nseq = (seq_t)n;

    for (size_t i = 0; i < nd; ++i) {
        seq_t d = divs[i];
        seq_t f = P / d;                     /* f = P / d, the complementary divisor */
        if ((d & 1) == (f & 1)) continue;    /* d + f must be odd to obtain integer m, k */
        if (f < d + 1) continue;             /* would give a negative k */

        seq_t k = (f - d - 1) / 2;            /* derived from equations */
        if (k >= nseq) count++;              /* ensure n <= k <= m  (m >= k automatically) */
    }

    return count;
}
