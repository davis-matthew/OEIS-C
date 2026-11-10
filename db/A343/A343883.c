/*
 * A343883  Numbers k such that A023896(k) is a multiple of A340179(k).
 * https://oeis.org/A343883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search k = 1, 2, ... for A023896(k) = k*phi(k)/2 divisible by
 *            A340179(k) = Sum_{x<k, gcd(x,k)=1} (A023896(k) mod x), with
 *            A340179(k) > 0; a(n) = n-th such k (conjectured to have only 8
 *            terms, so the search is capped and returns 0 beyond them)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A343883_gcd(idx_t a, idx_t b) {
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

static bool A343883_test(idx_t k) {
    idx_t phi = 0;
    for (idx_t x = 1; x < k; ++x)
        if (A343883_gcd(x, k) == 1) ++phi;
    seq_t c = (seq_t)k * (seq_t)phi / 2;
    seq_t sum = 0;
    for (idx_t x = 1; x < k; ++x)
        if (A343883_gcd(x, k) == 1) sum += c % (seq_t)x;
    if (sum == 0) return false;
    return c % sum == 0;
}

seq_t a(idx_t n) {
    static idx_t found[64];
    static idx_t cnt = 0;
    static idx_t next_k = 1;
    const idx_t LIMIT = 3000;
    if (n < 1 || n > 64) return (seq_t)0;
    while (cnt < n && next_k <= LIMIT) {
        if (A343883_test(next_k)) found[cnt++] = next_k;
        ++next_k;
    }
    if (n <= cnt) return (seq_t)found[n - 1];
    return (seq_t)0;
}
