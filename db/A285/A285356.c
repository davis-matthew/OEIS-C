/*
 * A285356  Numbers n such that the entries in the n-th row of the irregular
 *          triangle A237591 are in nonincreasing order.
 * https://oeis.org/A285356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that row m of A237591 is nonincreasing, where
 *            T(m,k) = ceil((m+1)/k-(k+1)/2) - ceil((m+1)/(k+1)-(k+2)/2),
 *            1<=k<=floor((sqrt(8m+1)-1)/2); computed by search over m,
 *            checking each row from its end (fails fast)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A285356_cdiv(int64_t num, int64_t den)
{
    if (num >= 0) return (num + den - 1) / den;
    return num / den;
}

static int64_t A285356_T(int64_t m, int64_t k)
{
    int64_t A = A285356_cdiv(2 * (m + 1) - k * (k + 1), 2 * k);
    int64_t B = A285356_cdiv(2 * (m + 1) - (k + 1) * (k + 2), 2 * (k + 1));
    return A - B;
}

seq_t a(idx_t n)
{
    int64_t cnt = 0;
    int64_t L = 0;
    int64_t m;
    for (m = 1; m <= 5000000; m++) {
        while ((L + 1) * (L + 2) / 2 <= m) L++;
        int ok = 1;
        if (L >= 2) {
            int64_t next = A285356_T(m, L);
            int64_t k;
            for (k = L - 1; k >= 1; k--) {
                int64_t cur = A285356_T(m, k);
                if (cur < next) { ok = 0; break; }
                next = cur;
            }
        }
        if (ok) {
            cnt++;
            if ((idx_t)cnt == n) return (seq_t)m;
        }
    }
    return 0;
}
