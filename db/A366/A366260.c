/*
 * A366260  Doudna sequence permuted by May code: a(n) =
 *          A005940(1+A303767(n)).
 * https://oeis.org/A366260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005940(1+A303767(n)); A303767(0) = 0, A303767(2^k) = 2^k
 *            + A303767(2^k-1), A303767(2^k+r) = 2^k + A303767(r-1) for
 *            0<r<2^k; A005940(n) = f(n-1,1,1) with f(0,i,x)=x,
 *            f(2m,i,x)=f(m,i+1,x), f(2m+1,i,x)=f(m,i,x*prime(i))
 * Range:     u8 n <= 31, u16 n <= 482, u32 n <= 15864, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 92 terms through n = 9792 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t may_code_A303767(uint64_t n) {
    uint64_t res = 0;
    while (n > 0) {
        uint64_t p = 1;
        while (p <= n / 2) p <<= 1;
        if (p == n) {
            res += n;
            n -= 1;
        } else {
            res += p;
            n = n - p - 1;
        }
    }
    return res;
}

static uint64_t next_prime_A005940(uint64_t c) {
    for (;;) {
        c++;
        bool ok = (c >= 2);
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

static seq_t doudna_A005940(uint64_t n) {
    uint64_t m = n - 1;
    seq_t result = 1;
    uint64_t pr = 2;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)pr;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            pr = next_prime_A005940(pr);
        }
    }
    return result;
}

seq_t a(idx_t n) {
    return doudna_A005940(1 + may_code_A303767((uint64_t)n));
}
