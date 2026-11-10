/*
 * A340525  Triangle read by rows: T(n,k) = A006218(n-k+1)*A002865(k-1), 1 <=
 *          k <= n.
 * https://oeis.org/A340525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A006218(n-k+1)*A002865(k-1), 1<=k<=n, read by rows;
 *            a(m) is the flat-index term. A006218(n)=Sum_{i=1..n} floor(n/i);
 *            A002865(j)=p(j)-p(j-1), A002865(0)=1, p by Euler's pentagonal
 *            recurrence
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t T_divsum(idx_t n) {
    idx_t s = 0;
    while ((s + 1) * (s + 1) <= n) s++;
    seq_t r = 0;
    for (idx_t i = 1; i <= s; i++) r += (seq_t)(n / i);
    r = r * 2 - (seq_t)s * (seq_t)s;
    return r;
}

static seq_t T_nopart1(idx_t j) {
    if (j == 0) return 1;
    seq_t *p = mem_new(seq_t, (size_t)(j + 1));
    p[0] = 1;
    for (idx_t m = 1; m <= j; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += p[m - g1];
                if (g2 <= m) pos += p[m - g2];
            } else {
                neg += p[m - g1];
                if (g2 <= m) neg += p[m - g2];
            }
        }
        p[m] = pos - neg;
    }
    seq_t res = p[j] - p[j - 1];
    mem_free(p);
    return res;
}

seq_t a(idx_t m) {
    idx_t r = 1;
    while (r * (r + 1) / 2 < m) r++;
    idx_t k = m - r * (r - 1) / 2;
    return T_divsum(r - k + 1) * T_nopart1(k - 1);
}
