/*
 * A324395  a(n) = A017666(A005940(1+n)), where A005940 is the Doudna sequence
 *          and A017666(n) = n/gcd(n,sigma(n)).
 * https://oeis.org/A324395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A017666(A005940(1+n)) = x/gcd(x, sigma(x)) with x =
 *            A005940(1+n); gcd is computed prime-by-prime from p-adic
 *            valuations of sigma(q^e) factors
 * Range:     u8 n <= 55, u16 n <= 487, u32 n <= 15855, u64 n <= 16500,
 *            u128 n <= 16500, big any n
 * Validated: standard, pass, 105 terms through n = 9887 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    int ex[72];
    uint64_t pr[72];
    int cnt = 0;
    for (uint64_t c = 2; cnt < 72; c++) {
        bool ok = true;
        for (int j = 0; j < cnt; j++) {
            if (pr[j] * pr[j] > c) break;
            if (c % pr[j] == 0) { ok = false; break; }
        }
        if (ok) { pr[cnt] = c; ex[cnt] = 0; cnt++; }
    }

    /* Doudna decoding of n = A005940(1+n) exponent vector */
    idx_t m = n;
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t D = (seq_t)1;
    for (int ai = 0; ai < cnt; ai++) {
        int e = ex[ai];
        if (e == 0) continue;
        uint64_t p = pr[ai];
        /* modulus M = p^c, c <= e, kept below 2^62 */
        uint64_t M = 1;
        int c = 0;
        while (c < e && M <= (((uint64_t)1 << 62) / p)) { M *= p; c++; }

        int v = 0; /* v_p(sigma(x)), capped */
        for (int k = 0; k < cnt && v < e; k++) {
            if (ex[k] == 0 || k == ai) continue;
            uint64_t r = 0, t = 1 % M;
            for (int j = 0; j <= ex[k]; j++) {
                r = (r + t) % M;
                t = (uint64_t)(((unsigned __int128)t * pr[k]) % M);
            }
            if (r == 0) {
                v += c;
            } else {
                while (r % p == 0) { r /= p; v++; }
            }
        }
        int keep = e - (v < e ? v : e);
        for (int j = 0; j < keep; j++) D *= (seq_t)p;
    }
    return D;
}
