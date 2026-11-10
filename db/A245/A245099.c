/*
 * A245099  Triangle read by rows: T(n,k) = A024916(k)*A002865(n-k).
 * https://oeis.org/A245099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A024916(k)*A002865(n-k), 1<=k<=n, read by rows;
 *            A024916(k) = Sum_{j=1..k} j*floor(k/j), A002865(m) =
 *            p(m)-p(m-1), A002865(0)=1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t T245099_sigsum(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t q = (seq_t)(n / k);
        res += ((seq_t)k) * q;
    }
    return res;
}

static seq_t T245099_noones(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t *p = mem_new(seq_t, (size_t)(n + 1));
    p[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
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
    seq_t result = p[n] - p[n - 1];
    mem_free(p);
    return result;
}

seq_t a(idx_t n)
{
    idx_t r = 1;
    idx_t start = 1; /* index of first entry of row r */
    while (start + r <= n) {
        start += r;
        r++;
    }
    idx_t k = n - start + 1;
    return T245099_sigsum(k) * T245099_noones(r - k);
}
