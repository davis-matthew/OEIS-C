/*
 * A324056  a(n) = A000593(A005940(1+n)).
 * https://oeis.org/A324056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000593(A005940(1+n)); computed as the product over odd
 *            primes p^e || A005940(1+n) of (1+p+...+p^e), with exponents read
 *            off the binary expansion of n
 * Range:     u8 n <= 51, u16 n <= 467, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 100 terms through n = 8139 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned e[72];
    unsigned long pr[72];
    int i, cnt = 0;
    for (i = 0; i < 72; i++) e[i] = 0;
    for (unsigned long c = 2; cnt < 72; c++) {
        int ok = 1;
        for (unsigned long d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) pr[cnt++] = c;
    }
    idx_t m = n;
    int k = 1;
    while (m > 0) {
        if (m & 1) {
            e[k]++;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            k++;
        }
    }
    seq_t result = 1;
    for (i = 2; i <= 71; i++) {
        if (e[i] == 0) continue;
        seq_t term = 1, pw = 1;
        for (unsigned j = 0; j < e[i]; j++) {
            pw *= (seq_t)pr[i - 1];
            term += pw;
        }
        result *= term;
    }
    return result;
}
