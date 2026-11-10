/*
 * A251717  a(n) = smallest positive integer k such that A083221(k, n) has at
 *          most two prime factors (is a prime or semiprime).
 * https://oeis.org/A251717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least k such that the n-th number m coprime to
 *            the first k-1 primes is 1 or prime, so that A083221(k,n) =
 *            prime(k)*m has at most two prime factors
 * Range:     u8 n <= 2518, u16 n <= 2518, u32 n <= 2518, u64 n <= 2518,
 *            u128 n <= 2518, big any n
 * Validated: standard, pass, 136 terms through n = 2516 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static int A251717_pr[100];
static int A251717_np = 0;

static void A251717_init(void)
{
    if (A251717_np) return;
    int c = 0;
    for (int x = 2; c < 100; x++) {
        bool ok = true;
        for (int i = 0; i < c && A251717_pr[i] * A251717_pr[i] <= x; i++)
            if (x % A251717_pr[i] == 0) { ok = false; break; }
        if (ok) A251717_pr[c++] = x;
    }
    A251717_np = c;
}

seq_t a(idx_t n)
{
    A251717_init();
    for (int k = 1; k <= A251717_np; k++) {
        idx_t cnt = 0;
        uint64_t m = 0;
        while (cnt < n) {
            m++;
            bool ok = true;
            for (int i = 0; i < k - 1; i++)
                if (m % (uint64_t)A251717_pr[i] == 0) { ok = false; break; }
            if (ok) cnt++;
        }
        if (m == 1 || is_prime((idx_t)m)) return (seq_t)k;
    }
    return 0;
}
