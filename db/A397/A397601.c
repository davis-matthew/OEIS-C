/*
 * A397601  Triangle read by rows: T(n, k) = T(n, k+1) + T(n-1, k) with
 *          initial values T(n, n) = A005043(n).
 * https://oeis.org/A397601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n,k+1) + T(n-1,k), T(n,n) = A005043(n), A005043(n) =
 *            (n-1)*(2*a(n-1)+3*a(n-2))/(n+1), A005043(0)=1, A005043(1)=0;
 *            a(m) = T(r,k) where m = r(r+1)/2 + k, 0<=k<=r
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void fill_riordan(seq_t *R, idx_t r) {
    R[0] = 1;
    if (r >= 1) R[1] = 0;
    for (idx_t i = 2; i <= r; i++) {
        idx_t g = (i & 1) ? 2 : 1;
        seq_t p = (seq_t)((i - 1) / g);
        seq_t m = (seq_t)((i + 1) / g);
        seq_t q1 = R[i - 1] / m, r1 = R[i - 1] % m;
        seq_t q2 = R[i - 2] / m, r2 = R[i - 2] % m;
        seq_t q = 2 * q1 + 3 * q2 + (2 * r1 + 3 * r2) / m;
        R[i] = p * q;
    }
}

seq_t a(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    seq_t *R = mem_new(seq_t, (size_t)(r + 2));
    seq_t *prev = mem_new(seq_t, (size_t)(r + 2));
    seq_t *cur = mem_new(seq_t, (size_t)(r + 2));
    fill_riordan(R, r);

    prev[0] = 1; /* row 0 */
    for (idx_t row = 1; row <= r; row++) {
        cur[row] = R[row];
        for (idx_t j = row; j-- > 0;) {
            cur[j] = cur[j + 1] + prev[j];
        }
        for (idx_t j = 0; j <= row; j++) prev[j] = cur[j];
    }
    seq_t res = (r == 0) ? prev[0] : cur[k];
    mem_free(R);
    mem_free(prev);
    mem_free(cur);
    return res;
}
