/*
 * A349248  Numbers which are elements of a cycle under iterations of the
 *          digit-census function A348783.
 * https://oeis.org/A349248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n-th k >= 1 lying on a cycle of A348783 (k = A348783^m(k)
 *            for some m >= 1); found by search over k, which stops at 2*10^6
 *            (a(9) > 10^6)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A349248_f(uint64_t n)
{
    unsigned cnt[10] = {0};
    uint64_t t = n;
    if (t == 0) cnt[0] = 1;
    while (t > 0) { cnt[t % 10]++; t /= 10; }
    uint64_t res = 0;
    for (int d = 9; d >= 0; --d) {
        uint64_t c = cnt[d];
        uint64_t sh = 10;
        while (sh <= c) sh *= 10;
        res = res * sh + c;
    }
    return res;
}

static int A349248_incycle(uint64_t k)
{
    uint64_t x = k;
    for (int i = 0; i < 40; i++) {
        x = A349248_f(x);
        if (x == k) return 1;
        if (x > 100000000000ULL) return 0;
    }
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t found[64];
    static int nfound = 0;
    static uint64_t next_k = 1;
    if (n < 1 || n > 63) return 0;
    while ((idx_t)nfound < n && next_k <= 2000000ULL) {
        if (A349248_incycle(next_k)) found[nfound++] = next_k;
        next_k++;
    }
    if ((idx_t)nfound < n) return 0;
    return (seq_t)found[n - 1];
}
