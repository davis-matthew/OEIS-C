/*
 * A368906  a(n) is the number of integers k in the range 0..n such that
 *          A342001(A005940(1+k)) is not divisible by p^p for any prime p.
 * https://oeis.org/A368906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k in 0..n : A342001(A005940(1+k)) not divisible by p^p
 *            for any prime p}; computed by direct count over k, with A005940
 *            exponents from the binary digits of k
 * Range:     u8 n <= 334, u16 n <= 2938, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 98 terms through n = 9307 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int A368906_primes_init = 0;
static uint64_t A368906_pr[64];

static void A368906_init(void)
{
    if (A368906_primes_init) return;
    int c = 0;
    for (uint64_t x = 2; c < 64; ++x) {
        int ok = 1;
        for (uint64_t d = 2; d * d <= x; ++d)
            if (x % d == 0) { ok = 0; break; }
        if (ok) A368906_pr[c++] = x;
    }
    A368906_primes_init = 1;
}

seq_t a(idx_t n)
{
    A368906_init();
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned e[65];
        for (int j = 0; j < 65; ++j) e[j] = 0;
        idx_t m = k;
        int i = 0;
        while (m > 0) {
            if (m & 1) { e[i]++; m >>= 1; }
            else { m >>= 1; i++; }
        }
        seq_t rad = 1;
        for (int j = 0; j < 64; ++j)
            if (e[j]) rad *= (seq_t)A368906_pr[j];
        seq_t S = 0;
        for (int j = 0; j < 64; ++j)
            if (e[j]) S += (seq_t)e[j] * (rad / (seq_t)A368906_pr[j]);
        int bad = 0;
        for (int j = 0; j < 64; ++j) {
            uint64_t p = A368906_pr[j];
            if (S < (seq_t)p) break;
            seq_t t = S;
            uint64_t c = 0;
            while (c < p && t % (seq_t)p == 0) { t /= (seq_t)p; ++c; }
            if (c >= p) { bad = 1; break; }
        }
        if (!bad) ++count;
    }
    return count;
}
