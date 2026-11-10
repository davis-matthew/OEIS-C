/*
 * A347960  Numbers k for which A348036(k) > A007947(k).
 * https://oeis.org/A347960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with gcd(k, A003968(k)) > rad(k); equivalently k
 *            has primes p != q with p^2 | k, q^2 | k and p | q+1. Computed by
 *            a search over k
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A347960_test(uint64_t k)
{
    uint64_t ps[24];
    uint64_t es[24];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            ps[cnt] = p;
            es[cnt] = e;
            cnt++;
        }
    }
    if (m > 1) {
        ps[cnt] = m;
        es[cnt] = 1;
        cnt++;
    }
    for (int i = 0; i < cnt; i++) {
        if (es[i] < 2)
            continue;
        for (int j = 0; j < cnt; j++) {
            if (j == i || es[j] < 2)
                continue;
            if ((ps[j] + 1) % ps[i] == 0)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    uint64_t target = (uint64_t)n;
    while (count < target) {
        k++;
        if (A347960_test(k))
            count++;
    }
    return (seq_t)k;
}
