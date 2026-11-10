/*
 * A365430  a(n) = A156552(gcd(n, A005940(n))), and A156552 is the inverse of
 *          offset-0 version of Doudna-sequence A005940.
 * https://oeis.org/A365430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A156552(gcd(n, A005940(n))); gcd is computed prime by
 *            prime from the Doudna exponents, and A156552 uses a(1)=0,
 *            a(2m)=1+2a(m), a(odd m>1)=2*a(A064989(m))
 * Range:     u8 n <= 319, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 8754 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A365430_isprime(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A365430_prevprime(uint64_t p) {
    /* largest prime < p, or 1 if none (p = 2) */
    uint64_t q = p - 1;
    while (q >= 2 && !A365430_isprime(q)) q--;
    return q < 2 ? 1 : q;
}

/* A156552: inverse of offset-0 Doudna sequence */
static uint64_t A365430_A156552(uint64_t m) {
    if (m == 1) return 0;
    if (m % 2 == 0) return 1 + 2 * A365430_A156552(m / 2);
    uint64_t prod = 1, x = m;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) {
            prod *= A365430_prevprime(d);
            x /= d;
        }
    }
    if (x > 1) prod *= A365430_prevprime(x);
    return 2 * A365430_A156552(prod);
}

seq_t a(idx_t n) {
    enum { K = 70 };
    uint64_t primes[K + 1];
    int cnt = 0;
    for (uint64_t c = 2; cnt < K; c++) {
        if (A365430_isprime(c)) primes[++cnt] = c;
    }
    int ex[K + 2];
    for (int i = 0; i <= K + 1; i++) ex[i] = 0;

    uint64_t m = (uint64_t)n - 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            if (i <= K) ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    uint64_t g = 1;
    for (int k = 1; k <= K; k++) {
        if (ex[k] == 0) continue;
        uint64_t p = primes[k];
        uint64_t t = (uint64_t)n;
        int v = 0;
        while (t % p == 0) { t /= p; v++; }
        int e = v < ex[k] ? v : ex[k];
        while (e-- > 0) g *= p;
    }
    return (seq_t)A365430_A156552(g);
}
