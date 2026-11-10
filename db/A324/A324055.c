/*
 * A324055  Deficiency of Doudna-sequence: a(n) = A033879(A005940(1+n)).
 * https://oeis.org/A324055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A033879(A005940(1+n)) = 2*A005940(1+n) -
 *            sigma(A005940(1+n))
 * Range:     i8 n <= 51, i16 n <= 471, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Validated: standard, pass, 98 terms through n = 7947 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    int e[72];
    uint64_t pr[72];
    int cnt = 0;
    for (uint64_t c = 2; cnt < 72; ++c) {
        bool ok = true;
        for (int j = 0; j < cnt && pr[j] * pr[j] <= c; ++j)
            if (c % pr[j] == 0) { ok = false; break; }
        if (ok) pr[cnt++] = c;
    }
    for (int i = 0; i < 72; ++i) e[i] = 0;

    /* Doudna: A005940(1+n), exponents from binary expansion of n */
    idx_t k = n;
    int i = 0;
    while (k > 0) {
        if (k & 1) {
            e[i]++;
            k = (k - 1) / 2;
        } else {
            k /= 2;
            i++;
        }
    }

    seq_t m = 1, sg = 1;
    for (int j = 0; j <= i && j < 72; ++j) {
        if (e[j] == 0) continue;
        seq_t p = (seq_t)pr[j];
        seq_t pw = 1, s = 1;
        for (int t = 0; t < e[j]; ++t) {
            pw *= p;
            s += pw;
        }
        m *= pw;
        sg *= s;
    }
    return (seq_t)2 * m - sg;
}
