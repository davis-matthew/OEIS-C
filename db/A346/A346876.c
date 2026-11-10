/*
 * A346876  Irregular triangle read by rows in which row n is the "n-th even
 *          perfect number" row of A237591, n >= 1.
 * https://oeis.org/A346876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(P,k) = ceil((P+1)/k-(k+1)/2) -
 *            ceil((P+1)/(k+1)-(k+2)/2), the k-th entry of row P of A237591,
 *            where P is the m-th even perfect number 2^(p-1)*(2^p-1) and row
 *            m of this triangle has length 2^p-1
 * Range:     u8 n <= 10, u16 n <= 168, u32 n <= 8359, u64 n <= 8359,
 *            u128 n <= 8359, big any n
 * Validated: standard, pass, 91 terms through n = 7823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned ex[] = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127};
    idx_t cum = 0;
    unsigned i = 0, p = 0;
    idx_t len = 0;

    if (n < 1) return 0;
    for (i = 0; i < sizeof(ex) / sizeof(ex[0]); i++) {
        p = ex[i];
        if (p >= 63) { len = ~(idx_t)0; }
        else len = (((idx_t)1) << p) - 1;
        if (len == ~(idx_t)0 || n - cum <= len) break;
        cum += len;
    }
    idx_t k = n - cum;

    seq_t P = 1;
    P <<= (p - 1);
    seq_t M = 1;
    M <<= p;
    M -= 1;
    P = P * M;

    seq_t K = (seq_t)k;
    seq_t r2 = (P + 1) * 2;
    seq_t numA = r2 - K * (K + 1);
    seq_t denA = K * 2;
    seq_t A = (numA + denA - 1) / denA;

    seq_t t = (K + 1) * (K + 2);
    seq_t denB = (K + 1) * 2;
    if (r2 >= t) {
        seq_t B = (r2 - t + denB - 1) / denB;
        return A - B;
    } else {
        seq_t m = t - r2;
        return A + m / denB;
    }
}
