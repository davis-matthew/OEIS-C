/*
 * A246271  Starting from A003961(n), the number of additional iterations of
 *          A003961 required for the result to be of the form 4k+1.
 * https://oeis.org/A246271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if A003961(n) == 1 (mod 4), else 1 + a(A003961(n));
 *            computed by shifting the prime factors of n to the next prime
 *            until the count of odd-exponent primes == 3 (mod 4) is even
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 139 terms through n = 9799 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    uint64_t q[64];
    int odd[64];
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            q[cnt] = p;
            odd[cnt] = e & 1;
            cnt++;
        }
    }
    if (m > 1) {
        q[cnt] = m;
        odd[cnt] = 1;
        cnt++;
    }
    unsigned long long t = 0;
    for (;;) {
        int parity = 0;
        for (int i = 0; i < cnt; i++) {
            q[i] = next_prime(q[i]);
            if (odd[i] && (q[i] & 3ULL) == 3ULL) parity ^= 1;
        }
        if (parity == 0) return (seq_t)t;
        t++;
    }
}
