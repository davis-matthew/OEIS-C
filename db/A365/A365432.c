/*
 * A365432  a(n) = A156552(A364502(n)), where A364502(n) = A005940(n) / gcd(n,
 *          A005940(n)), and A156552 is the inverse of offset-0 version of
 *          Doudna-sequence A005940.
 * https://oeis.org/A365432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A156552(A005940(n)/gcd(n, A005940(n))); computed via
 *            prime-exponent vectors: exponents of A005940(n) read from n-1 in
 *            binary, divided by gcd with n, then re-encoded as the Doudna
 *            inverse
 * Range:     u8 n <= 256, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 8456 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A365432_primes(uint64_t *pr, int cnt) {
    int c = 0;
    uint64_t x = 2;
    while (c < cnt) {
        bool ok = true;
        for (int j = 0; j < c; j++) {
            if (pr[j] * pr[j] > x) break;
            if (x % pr[j] == 0) { ok = false; break; }
        }
        if (ok) pr[c++] = x;
        x++;
    }
}

seq_t a(idx_t n) {
    enum { NP = 80 };
    uint64_t pr[NP];
    uint64_t e[NP + 2];
    A365432_primes(pr, NP);
    for (int k = 0; k < NP + 2; k++) e[k] = 0;

    /* exponents of A005940(n) */
    idx_t m = n - 1;
    int i = 0;
    while (m > 0) {
        if (m & 1) { e[i]++; m >>= 1; }
        else { m >>= 1; i++; }
    }
    /* divide by gcd with n */
    for (int k = 0; k < NP; k++) {
        if (e[k] == 0) continue;
        uint64_t t = (uint64_t)n, c = 0;
        while (t % pr[k] == 0) { t /= pr[k]; c++; }
        uint64_t r = c < e[k] ? c : e[k];
        e[k] -= r;
    }
    int K = -1;
    for (int k = 0; k < NP; k++) if (e[k] > 0) K = k;
    seq_t res = 0;
    if (K < 0) return res;

    /* bits LSB-first: e_1 ones, 0, e_2 ones, 0, ..., e_K ones */
    static unsigned char bits[512];
    int L = 0;
    for (int k = 0; k <= K; k++) {
        for (uint64_t j = 0; j < e[k]; j++) bits[L++] = 1;
        if (k < K) bits[L++] = 0;
    }
    for (int p = L - 1; p >= 0; p--) {
        res = res * 2 + (seq_t)bits[p];
    }
    return res;
}
