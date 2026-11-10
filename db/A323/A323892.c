/*
 * A323892  Lexicographically earliest sequence such that a(i) = a(j) =>
 *          A002487(i) = A002487(j) and A033879(i) = A033879(j), for all i, j
 *          >= 1.
 * https://oeis.org/A323892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A002487(n),
 *            A033879(n)]: a(n) = 1 + number of distinct pairs first seen
 *            before the first occurrence of the pair at n (computed by
 *            scanning k = 1..n)
 * Range:     u8 n <= 278, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9900 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A323892_stern(idx_t n) {
    seq_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

/* deficiency 2k - sigma(k) as sign flag and magnitude */
static void A323892_def(idx_t k, seq_t *flag, seq_t *mag) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) {
            idx_t j = k / i;
            sum += (seq_t)i;
            if (i != j) sum += (seq_t)j;
        }
    }
    seq_t two_k = (seq_t)2 * (seq_t)k;
    if (two_k >= sum) { *flag = 0; *mag = two_k - sum; }
    else { *flag = 1; *mag = sum - two_k; }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t *S = mem_new(seq_t, (size_t)n);
    seq_t *F = mem_new(seq_t, (size_t)n);
    seq_t *M = mem_new(seq_t, (size_t)n);
    size_t cnt = 0;
    size_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t s = A323892_stern(k);
        seq_t f, m;
        A323892_def(k, &f, &m);
        size_t found = 0;
        bool ok = false;
        for (size_t t = 0; t < cnt; ++t) {
            if (S[t] == s && M[t] == m && F[t] == f) { found = t + 1; ok = true; break; }
        }
        if (!ok) {
            S[cnt] = s; F[cnt] = f; M[cnt] = m;
            cnt++;
            found = cnt;
        }
        res = found;
    }
    mem_free(S);
    mem_free(F);
    mem_free(M);
    return (seq_t)res;
}
