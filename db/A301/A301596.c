/*
 * A301596  Expansion of Product_{k>=1} (1 + x^k)^A050985(k).
 * https://oeis.org/A301596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Product_{k>=1} (1 + x^k)^A050985(k); computed by
 *            multiplying the truncated power series by each (1+x^k)^e, where
 *            e = A050985(k) and (1+y)^e is found by binary exponentiation.
 *            There is no division, so the values are exact (taken mod 2^w for
 *            fixed-width seq_t)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 56, u64 n <= 146,
 *            u128 n <= 392, big any n
 * Validated: standard, pass, 56 terms through n = 886 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* cubefree part of n, native arithmetic (used only as an exponent) */
static uint64_t A301596_A050985_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = 1, m = n;
    if ((m & 1) == 0) {
        uint64_t cnt = 0;
        while ((m & 1) == 0) { m >>= 1; cnt++; }
        if (cnt % 3 == 1) result *= 2;
        else if (cnt % 3 == 2) result *= 4;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) { m /= p; cnt++; }
            if (cnt % 3 == 1) result *= p;
            else if (cnt % 3 == 2) result *= p * p;
        }
    }
    if (m > 1) result *= m;
    return result;
}

/* dst = (x * y) truncated to degree m; tmp is scratch of size m+1 */
static void A301596_mul(seq_t *dst, const seq_t *x, const seq_t *y,
                        size_t m, seq_t *tmp)
{
    for (size_t i = 0; i <= m; i++) tmp[i] = 0;
    for (size_t i = 0; i <= m; i++) {
        if (x[i] == 0) continue;
        for (size_t j = 0; i + j <= m; j++)
            tmp[i + j] = tmp[i + j] + x[i] * y[j];
    }
    for (size_t i = 0; i <= m; i++) dst[i] = tmp[i];
}

static seq_t *A301596_tab = 0;
static size_t A301596_cnt = 0;

seq_t a(idx_t n)
{
    size_t want = (size_t)n + 1;
    if (want > A301596_cnt) {
        size_t N = want;
        if (N < 2 * A301596_cnt) N = 2 * A301596_cnt;
        if (N < 64) N = 64;
        size_t D = N - 1; /* max degree */
        seq_t *cur = mem_new(seq_t, N);
        seq_t *res = mem_new(seq_t, N);
        seq_t *base = mem_new(seq_t, N);
        seq_t *tmp = mem_new(seq_t, N);
        cur[0] = 1;
        for (size_t k = 1; k <= D; k++) {
            uint64_t e = A301596_A050985_u64((uint64_t)k);
            size_t m = D / k;
            for (size_t i = 0; i <= m; i++) { res[i] = 0; base[i] = 0; }
            res[0] = 1;
            base[0] = 1;
            if (m >= 1) base[1] = 1;
            while (e) {
                if (e & 1) A301596_mul(res, res, base, m, tmp);
                e >>= 1;
                if (e) A301596_mul(base, base, base, m, tmp);
            }
            /* cur *= sum_j res[j] x^{k j}, in place, descending */
            for (size_t i = D + 1; i-- > 0;) {
                seq_t s = cur[i];
                size_t jm = i / k;
                for (size_t j = 1; j <= jm; j++)
                    s = s + res[j] * cur[i - k * j];
                cur[i] = s;
            }
        }
        mem_free(res);
        mem_free(base);
        mem_free(tmp);
        if (A301596_tab) mem_free(A301596_tab);
        A301596_tab = cur;
        A301596_cnt = N;
    }
    return A301596_tab[(size_t)n];
}
