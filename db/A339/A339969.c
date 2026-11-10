/*
 * A339969  a(n) = gcd(n, A005940(1+n)).
 * https://oeis.org/A339969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A005940(1+n)), computed per prime: min(v_p(n),
 *            v_p(A005940(1+n)))
 * Range:     u8 n <= 749, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 8976 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A339969_primes(uint64_t *pr, int cnt) {
    int c = 0;
    uint64_t x = 2;
    while (c < cnt) {
        bool ok = true;
        for (int j = 0; j < c && pr[j] * pr[j] <= x; j++) {
            if (x % pr[j] == 0) { ok = false; break; }
        }
        if (ok) pr[c++] = x;
        x++;
    }
}

seq_t a(idx_t n) {
    uint64_t pr[72];
    unsigned e[72];
    A339969_primes(pr, 72);
    for (int k = 0; k < 72; k++) e[k] = 0;

    /* exponents of A005940(1+n): binary expansion of n */
    idx_t m = n;
    int i = 1;
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
    if (n == 0) return result;
    for (int k = 1; k <= i && k < 72; k++) {
        if (e[k] == 0) continue;
        uint64_t p = pr[k - 1];
        idx_t t = n;
        unsigned v = 0;
        while (t % p == 0) { t /= p; v++; }
        unsigned f = v < e[k] ? v : e[k];
        for (unsigned j = 0; j < f; j++) result *= (seq_t)p;
    }
    return result;
}
