/*
 * A337375  a(n) = A330749(A005940(1+n)).
 * https://oeis.org/A337375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A330749(A005940(1+n)); with A005940(1+n) = prod
 *            prime(k)^e_k, a(n) = prod prime(k)^min(e_k, e_{k+1})
 * Range:     u8 n <= 339, u16 n <= 5459, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 122 terms through n = 9071 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    int e[72];
    int pr[72];
    int i;
    for (i = 0; i < 72; i++) e[i] = 0;

    /* first 72 primes (indices 1..72) */
    int cnt = 0;
    for (int c = 2; cnt < 72; c++) {
        bool ok = true;
        for (int d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) pr[++cnt - 1] = c;
    }

    idx_t m = n;
    i = 1;
    while (m > 0) {
        if (m & 1) {
            e[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t result = 1;
    for (int k = 1; k < 70; k++) {
        int mn = e[k] < e[k + 1] ? e[k] : e[k + 1];
        for (int j = 0; j < mn; j++) result *= (seq_t)pr[k - 1];
    }
    return result;
}
