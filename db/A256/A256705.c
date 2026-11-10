/*
 * A256705  Numbers n such that if we define f(n,m) by the recursion f(n,1)=n,
 *          f(n,k+1)= A007672(n,k), the subsequence f(n,k), with n constant,
 *          is eventually periodic with period two.
 * https://oeis.org/A256705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that iterating f -> A002034(f)!/f from
 *            m never reaches 1; with K=A002034(f), this holds iff K does not
 *            divide f at some step (searched with prime-exponent vectors)
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t A256705_leg(idx_t p, idx_t k) {
    idx_t e = 0;
    while (k > 0) { k /= p; e += k; }
    return e;
}

static bool A256705_test(idx_t m) {
    if (m < 2) return false;
    idx_t *spf = (idx_t *)calloc((size_t)m + 1, sizeof(idx_t));
    idx_t *e = (idx_t *)calloc((size_t)m + 1, sizeof(idx_t));
    for (idx_t i = 2; i <= m; i++) {
        if (spf[i] == 0) {
            for (idx_t j = i; j <= m; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    idx_t t = m;
    while (t > 1) { e[spf[t]]++; t /= spf[t]; }
    idx_t cur = m;
    bool result = false;
    for (;;) {
        idx_t K = 1;
        for (idx_t p = 2; p <= cur; p++) {
            if (spf[p] == p && e[p] > 0) {
                idx_t k = p;
                while (A256705_leg(p, k) < e[p]) k += p;
                if (k > K) K = k;
            }
        }
        if (K == 1) { result = false; break; }
        bool divides = true;
        idx_t q = K;
        while (q > 1) {
            idx_t p = spf[q];
            idx_t c = 0;
            while (q % p == 0) { q /= p; c++; }
            if (e[p] < c) { divides = false; break; }
        }
        if (!divides) { result = true; break; }
        for (idx_t p = 2; p <= cur; p++) {
            if (spf[p] == p) {
                idx_t v = (p <= K) ? A256705_leg(p, K) : 0;
                e[p] = v - e[p];
            }
        }
        cur = K;
    }
    free(spf);
    free(e);
    return result;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 1;
    while (count < n) {
        m++;
        if (A256705_test(m)) count++;
    }
    return (seq_t)m;
}
