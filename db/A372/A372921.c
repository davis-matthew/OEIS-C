/*
 * A372921  Triangle read by rows: T(n, k) = (Sum_{i=0..n-k} (-1)^i *
 *          binomial(n-k, i) * A007559(n-i)) * n! / ((n-k)! * A007559(k)) for
 *          0 <= k <= n.
 * https://oeis.org/A372921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = (T(n-1,k-1) + 3*T(n-1,k))*n for 0<k<n,
 *            T(n,0)=A033030(n) (d(n)=3*(n-1)*(d(n-1)+d(n-2)), d(0)=1,
 *            d(1)=0), T(n,n)=n!; a(m) is T(n,k) read by rows
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t m) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= m) r++;
    idx_t k = m - r * (r + 1) / 2;

    seq_t *row = mem_new(seq_t, (size_t)r + 2);
    row[0] = 1;
    seq_t dprev2 = 1, dprev1 = 0; /* d(i-2), d(i-1) at i = 1 -> d(-1) unused */
    /* d(0)=1 */
    seq_t d0 = 1, d1 = 0;
    seq_t fact = 1;
    (void)dprev2; (void)dprev1;
    for (idx_t i = 1; i <= r; i++) {
        seq_t di;
        if (i == 1) di = d1;
        else {
            di = (seq_t)3 * (seq_t)(i - 1) * (d1 + d0);
            d0 = d1;
            d1 = di;
        }
        fact *= (seq_t)i;
        for (idx_t j = i - 1; j >= 1; j--) {
            row[j] = (row[j - 1] + (seq_t)3 * row[j]) * (seq_t)i;
        }
        row[i] = fact;
        row[0] = di;
    }
    seq_t res = row[k];
    mem_free(row);
    return res;
}
