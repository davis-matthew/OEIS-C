/*
 * A308106  Distinct values taken by the DENEAT operator (A073053) in order of
 *          appearance.
 * https://oeis.org/A308106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = DENEAT(m_n), where m_n runs over the first occurrences of
 *            each distinct DENEAT value. For t-digit numbers (t>=2), the
 *            minimal representatives 1 0^e 1^(o-1) (e+o=t, o>=1) and 2
 *            0^(t-1) give (e,o,t) with e = t-1,...,0, then e=t. a(1)=101,
 *            a(2)=11
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A308106_len(idx_t x)
{
    int l = 1;
    while (x >= 10) { x /= 10; l++; }
    return l;
}

static seq_t A308106_pow10(int k)
{
    seq_t r = 1;
    for (int i = 0; i < k; ++i) r *= 10;
    return r;
}

static seq_t A308106_make(idx_t e, idx_t o, idx_t t)
{
    int lo = A308106_len(o), lt = A308106_len(t);
    return (seq_t)e * A308106_pow10(lo + lt) + (seq_t)o * A308106_pow10(lt) + (seq_t)t;
}

seq_t a(idx_t n)
{
    if (n <= 1) return A308106_make(1, 0, 1);
    if (n == 2) return A308106_make(0, 1, 1);
    idx_t k = n - 3;
    idx_t t = 2;
    while (k >= t + 1) {
        k -= t + 1;
        t++;
    }
    idx_t e = (k < t) ? (t - 1 - k) : t;
    idx_t o = t - e;
    return A308106_make(e, o, t);
}
