/*
 * A351552  Numbers k for which A351546(k) divides k, where A351546(n) is the
 *          largest unitary divisor of sigma(n) such that none of its prime
 *          factors divide A003961(n).
 * https://oeis.org/A351552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A351546(k) divides k, where A351546(k)
 *            is the product of q^f over q^f || sigma(k) with q not dividing
 *            A003961(k); found by a sequential search over k, memoized
 * Range:     u8 n <= 12, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static bool A351552_test(uint64_t k) {
    uint64_t np[32];
    size_t pc = 0;
    uint64_t sigma = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t pw = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                s += pw;
            }
            sigma *= s;
            np[pc++] = next_prime(p);
        }
    }
    if (m > 1) {
        sigma *= (m + 1);
        np[pc++] = next_prime(m);
    }
    uint64_t res = 1;
    uint64_t t = sigma;
    for (uint64_t q = 2; q * q <= t || t > 1; q++) {
        if (q * q > t) q = t;
        if (t % q == 0) {
            uint64_t pw = 1;
            while (t % q == 0) {
                t /= q;
                pw *= q;
            }
            bool ex = false;
            for (size_t j = 0; j < pc; j++)
                if (np[j] == q) { ex = true; break; }
            if (!ex) res *= pw;
        }
    }
    return (k % res) == 0;
}

#define A351552_MAX 100000
static uint64_t A351552_terms[A351552_MAX];
static size_t A351552_cnt = 0;
static uint64_t A351552_next = 1;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A351552_cnt < n && A351552_cnt < A351552_MAX) {
        if (A351552_test(A351552_next))
            A351552_terms[A351552_cnt++] = A351552_next;
        A351552_next++;
    }
    return (seq_t)A351552_terms[n - 1];
}
