/*
 * A385348  Minimum number of products of the form P^2-i^2 to be used to
 *          obtain the GCD defined in A380472.
 * https://oeis.org/A385348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that gcd of Product_{i=1..n}(P^2-i^2) over
 *            the first k primes P >= prime(n+2) equals the full gcd of
 *            A380472, (2n+2)!*(3/4-(-1)^n/4). Computed by search over prime
 *            exponents
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A385348_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t lim = 2 * N + 1;
    /* small primes q <= 2n+1 */
    uint8_t *comp = (uint8_t *)calloc(lim + 2, 1);
    uint64_t *qs = (uint64_t *)malloc(sizeof(uint64_t) * (lim + 2));
    size_t nq = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            qs[nq++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    int *E = (int *)malloc(sizeof(int) * (nq + 1));
    int *mn = (int *)malloc(sizeof(int) * (nq + 1));
    for (size_t t = 0; t < nq; t++) {
        uint64_t q = qs[t];
        int e = 0;
        uint64_t m = 2 * N + 2;
        while (m) { m /= q; e += (int)m; }
        if (q == 2 && (N % 2 == 0)) e -= 1;
        E[t] = e;
        mn[t] = 1 << 30;
    }
    uint64_t *cand = (uint64_t *)malloc(sizeof(uint64_t) * (20 * N + 40));
    size_t nc = 0;

    /* find prime(n+2) */
    uint64_t P = 1;
    uint64_t cnt = 0;
    while (cnt < N + 2) {
        P++;
        if (A385348_isprime(P)) cnt++;
    }

    uint64_t k = 0;
    for (;;) {
        k++;
        /* exponents of small primes */
        for (size_t t = 0; t < nq; t++) {
            uint64_t q = qs[t];
            int v = 0;
            for (uint64_t i = 1; i <= N; i++) {
                uint64_t x = P * P - i * i;
                while (x % q == 0) { x /= q; v++; }
            }
            if (v < mn[t]) mn[t] = v;
        }
        if (k == 1) {
            /* candidate large primes dividing the first product */
            for (uint64_t i = 1; i <= N; i++) {
                for (int s = 0; s < 2; s++) {
                    uint64_t x = s ? P + i : P - i;
                    for (uint64_t d = 2; d * d <= x; d++) {
                        if (x % d == 0) {
                            while (x % d == 0) x /= d;
                            if (d > lim) {
                                bool f = false;
                                for (size_t z = 0; z < nc; z++) if (cand[z] == d) { f = true; break; }
                                if (!f) cand[nc++] = d;
                            }
                        }
                    }
                    if (x > 1 && x > lim) {
                        bool f = false;
                        for (size_t z = 0; z < nc; z++) if (cand[z] == x) { f = true; break; }
                        if (!f) cand[nc++] = x;
                    }
                }
            }
        } else {
            size_t w = 0;
            for (size_t z = 0; z < nc; z++) {
                uint64_t c = cand[z];
                bool div = false;
                for (uint64_t i = 1; i <= N; i++)
                    if ((P * P - i * i) % c == 0) { div = true; break; }
                if (div) cand[w++] = c;
            }
            nc = w;
        }
        bool ok = (nc == 0);
        for (size_t t = 0; ok && t < nq; t++)
            if (mn[t] != E[t]) ok = false;
        if (ok) break;
        do { P++; } while (!A385348_isprime(P));
    }
    free(comp); free(qs); free(E); free(mn); free(cand);
    return (seq_t)k;
}
