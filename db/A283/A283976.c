/*
 * A283976  a(2n) = A002487(n), a(2n+1) = A002487(n) OR A002487(n+1), where OR
 *          is bitwise-or (A003986).
 * https://oeis.org/A283976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2n) = A002487(n), a(2n+1) = A002487(n) | A002487(n+1) where
 *            A002487 is defined by A002487(0)=0, A002487(1)=1 and
 *            A002487(2k)=A002487(k), A002487(2k+1)=A002487(k)+A002487(k+1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc and free */

seq_t a(idx_t n) {
    /* maximum index needed for A002487 */
    idx_t pmax = (n + 2) / 2;   /* ceil((n+1)/2) */

    /* allocate array of seq_t */
    seq_t *p = (seq_t *)malloc((pmax + 1) * sizeof(seq_t));
    if (!p) return 0;  /* out of memory; return zero as safe default */

    /* base cases */
    p[0] = 0;
    if (pmax >= 1) p[1] = 1;

    /* compute A002487 up to pmax */
    for (idx_t i = 2; i <= pmax; ++i) {
        if ((i & 1) == 0) {          /* even */
            p[i] = p[i >> 1];
        } else {                     /* odd */
            p[i] = p[(i - 1) >> 1] + p[(i + 1) >> 1];
        }
    }

    seq_t result;
    if ((n & 1) == 0) {   /* even */
        result = p[n >> 1];
    } else {              /* odd */
        idx_t k = (n - 1) >> 1;
        result = p[k] | p[k + 1];
    }

    free(p);
    return result;
}
