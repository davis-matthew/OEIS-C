/*
 * A347891  Odd numbers k such that sigma(k^2) > 2*k^2 and A003415(sigma(k^2))
 *          < k^2.
 * https://oeis.org/A347891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m, where m is the n-th odd number with sigma(m^2) > 2*m^2
 *            and A003415(sigma(m^2)) < m^2 (A347891(n) =
 *            A000196(A347890(n))); found by an incremental search using a
 *            smallest-prime-factor sieve that doubles in size as needed
 * Range:     u8 none, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A347891_u128;

static uint32_t *A347891_spf = NULL;
static uint64_t A347891_limit = 0;
static uint64_t A347891_scanned = 1;
static uint64_t *A347891_res = NULL;
static size_t A347891_cnt = 0, A347891_cap = 0;

static void A347891_build(uint64_t L) {
    if (A347891_spf) free(A347891_spf);
    A347891_spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (A347891_spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (A347891_spf[j] == 0) A347891_spf[j] = (uint32_t)i;
        }
    }
    A347891_limit = L;
}

static bool A347891_test(uint64_t m) {
    uint64_t ps[32], term[32];
    int cnt = 0;
    uint64_t x = m;
    A347891_u128 S = 1;
    while (x > 1) {
        uint64_t p = A347891_spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        uint64_t t = 1, pw = 1;
        for (int i = 0; i < 2 * e; i++) { pw *= p; t += pw; }
        ps[cnt] = p; term[cnt] = t; cnt++;
        S *= t;
    }
    A347891_u128 k = (A347891_u128)m * m;
    if (!(S > 2 * k)) return false;
    A347891_u128 D = 0;
    for (int i = 0; i < cnt; i++) {
        uint64_t t = term[i];
        while (t % 2 == 0) { D += S / 2; t /= 2; }
        for (uint64_t d = 3; d * d <= t; d += 2) {
            while (t % d == 0) { D += S / d; t /= d; }
        }
        if (t > 1) D += S / t;
    }
    return D < k;
}

seq_t a(idx_t n) {
    while (A347891_cnt < (size_t)n) {
        uint64_t nl = A347891_limit * 2;
        if (nl < 65536) nl = 65536;
        A347891_build(nl);
        uint64_t m = A347891_scanned + 1;
        if ((m & 1ULL) == 0) m++;
        for (; m <= A347891_limit; m += 2) {
            if (m > 1 && A347891_test(m)) {
                if (A347891_cnt == A347891_cap) {
                    A347891_cap = A347891_cap ? A347891_cap * 2 : 256;
                    A347891_res = (uint64_t *)realloc(A347891_res, A347891_cap * sizeof(uint64_t));
                }
                A347891_res[A347891_cnt++] = m;
            }
        }
        A347891_scanned = A347891_limit;
    }
    return (seq_t)A347891_res[n - 1];
}
