/*
 * A387166  Numbers k for which gcd(k, A003961(k)) = gcd(sigma(k), A003961(k))
 *          > 1, and that satisfy Euler's condition for odd perfect numbers
 *          (A228058).
 * https://oeis.org/A387166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. k = p^e*m^2 (odd, p == e == 1 (mod 4), p prime not
 *            dividing m) is a term iff gcd(k, A003961(k)) = gcd(sigma(k),
 *            A003961(k)) > 1. Since A003961 maps each prime to the next one,
 *            gcd(k, A003961(k)) > 1 needs two consecutive primes dividing m,
 *            or p dividing A003961(m), or nextprime(p) dividin...
 * Range:     u8 none, u16 n <= 2, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 55 terms through n = 9592 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 A387166_u128;

static A387166_u128 A387166_gcd(A387166_u128 x, A387166_u128 y) {
    while (y) { A387166_u128 t = x % y; x = y; y = t; }
    return x;
}

static int A387166_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return u < v ? -1 : (u > v ? 1 : 0);
}

static uint64_t *A387166_res = 0;
static size_t A387166_cnt = 0;
static uint64_t A387166_limit = 0;

static size_t A387166_rc = 0, A387166_rcap = 0;
static uint64_t *A387166_tmp = 0;

/* bitset over odd numbers: bit i <-> 2i+1, set = composite */
static uint8_t *A387166_comp = 0;

static int A387166_isc(uint64_t x) { /* x odd */
    uint64_t i = x >> 1;
    return (A387166_comp[i >> 3] >> (i & 7)) & 1;
}

static uint64_t A387166_nextp(uint64_t p) {
    if (p < 2) return 2;
    if (p == 2) return 3;
    uint64_t x = p + 2;
    while (A387166_isc(x)) x += 2;
    return x;
}

static uint64_t A387166_prevp(uint64_t p) { /* p >= 5, odd */
    uint64_t x = p - 2;
    while (x > 3 && A387166_isc(x)) x -= 2;
    return x;
}

static void A387166_push(uint64_t k) {
    if (A387166_rc == A387166_rcap) {
        A387166_rcap = A387166_rcap ? A387166_rcap * 2 : 1024;
        A387166_tmp = (uint64_t *)realloc(A387166_tmp, A387166_rcap * sizeof(uint64_t));
    }
    A387166_tmp[A387166_rc++] = k;
}

static void A387166_try(uint64_t p, uint64_t s, uint64_t pmax,
                        A387166_u128 Am2, A387166_u128 sm) {
    uint64_t Ap = A387166_nextp(p);
    uint64_t pe = p;
    A387166_u128 Ape = Ap, spe = 1 + (A387166_u128)p;
    for (;;) {
        if (pe > pmax) break;
        uint64_t k = pe * s;
        A387166_u128 A = Ape * Am2;
        A387166_u128 g1 = A387166_gcd((A387166_u128)k, A);
        if (g1 > 1) {
            A387166_u128 g2 = A387166_gcd(spe * sm, A);
            if (g1 == g2) A387166_push(k);
        }
        int over = 0;
        for (int j = 0; j < 4; j++) {
            if (pe > pmax / p) { over = 1; break; }
            pe *= p; Ape *= Ap; spe = spe * p + 1;
        }
        if (over) break;
    }
}

static void A387166_build(uint64_t L) {
    uint64_t mmax = 1;
    while (5 * (mmax + 1) * (mmax + 1) <= L) mmax++;
    uint64_t top = L / 225;
    if (top < mmax) top = mmax;
    top += 4000;
    uint64_t half = top / 2 + 2;
    size_t bytes = (size_t)(half / 8 + 2);
    A387166_comp = (uint8_t *)calloc(bytes, 1);
    A387166_comp[0] |= 1; /* 1 not prime */
    for (uint64_t i = 1; (2 * i + 1) * (2 * i + 1) <= top; i++) {
        if (!((A387166_comp[i >> 3] >> (i & 7)) & 1)) {
            uint64_t p = 2 * i + 1;
            for (uint64_t j = (p * p) >> 1; j < half; j += p)
                A387166_comp[j >> 3] |= (uint8_t)(1u << (j & 7));
        }
    }
    A387166_rc = 0;
    for (uint64_t m = 3; m <= mmax; m += 2) {
        uint64_t qs[20], fs[20], qn[20]; int nq = 0;
        uint64_t t = m;
        for (uint64_t q = 3; q * q <= t; q += 2) {
            if (t % q == 0) {
                uint64_t f = 0;
                while (t % q == 0) { t /= q; f++; }
                qs[nq] = q; fs[nq] = f; nq++;
            }
        }
        if (t > 1) { qs[nq] = t; fs[nq] = 1; nq++; }
        A387166_u128 Am = 1, sm = 1;
        bool flag = false;
        for (int i = 0; i < nq; i++) {
            qn[i] = A387166_nextp(qs[i]);
            if (m % qn[i] == 0) flag = true;
            A387166_u128 sg = 1, pw = 1;
            for (uint64_t j = 0; j < fs[i]; j++) Am *= qn[i];
            for (uint64_t j = 0; j < 2 * fs[i]; j++) { pw *= qs[i]; sg += pw; }
            sm *= sg;
        }
        A387166_u128 Am2 = Am * Am;
        uint64_t s = m * m;
        uint64_t pmax = L / s;
        if (flag) {
            for (uint64_t p = 5; p <= pmax; p += 4) {
                if (A387166_isc(p)) continue;
                if (m % p == 0) continue;
                A387166_try(p, s, pmax, Am2, sm);
            }
        } else {
            uint64_t cand[40]; int nc = 0;
            for (int i = 0; i < nq; i++) {
                uint64_t c1 = qn[i];
                uint64_t c2 = qs[i] >= 5 ? A387166_prevp(qs[i]) : 0;
                uint64_t cs[2]; cs[0] = c1; cs[1] = c2;
                for (int z = 0; z < 2; z++) {
                    uint64_t c = cs[z];
                    if (c < 5 || (c & 3) != 1 || c > pmax || m % c == 0) continue;
                    bool dup = false;
                    for (int w = 0; w < nc; w++) if (cand[w] == c) { dup = true; break; }
                    if (!dup) cand[nc++] = c;
                }
            }
            for (int w = 0; w < nc; w++) A387166_try(cand[w], s, pmax, Am2, sm);
        }
    }
    free(A387166_comp);
    A387166_comp = 0;
    qsort(A387166_tmp, A387166_rc, sizeof(uint64_t), A387166_cmp);
    free(A387166_res);
    A387166_res = (uint64_t *)malloc((A387166_rc + 1) * sizeof(uint64_t));
    if (A387166_rc) memcpy(A387166_res, A387166_tmp, A387166_rc * sizeof(uint64_t));
    A387166_cnt = A387166_rc;
    A387166_limit = L;
}

seq_t a(idx_t n) {
    if (A387166_limit == 0) A387166_build((uint64_t)1 << 24);
    while ((uint64_t)n > A387166_cnt) A387166_build(A387166_limit * 2);
    return (seq_t)A387166_res[n - 1];
}
