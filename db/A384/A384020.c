/*
 * A384020  Numbers k > 0 such that sigma(A018804(k)) = k*tau(A018804(k))
 *          where sigma denotes the sum of divisors (A000203) and tau denotes
 *          the number of divisors (A000005).
 * https://oeis.org/A384020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k>0 with sigma(P(k)) = k*tau(P(k)), P = A018804
 *            (multiplicative, P(p^e) = p^(e-1)*((e+1)p-e)); found by search
 *            over k
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A384020_pillai(uint64_t k)
{
    uint64_t res = 1, p;
    for (p = 2; p * p <= k; p += (p == 2 ? 1 : 2)) {
        if (k % p == 0) {
            uint64_t e = 0, pe1 = 1;
            while (k % p == 0) {
                k /= p;
                e++;
                if (e > 1) pe1 *= p;
            }
            res *= pe1 * ((e + 1) * p - e);
        }
    }
    if (k > 1) res *= (2 * k - 1);
    return res;
}

static bool A384020_test(uint64_t k)
{
    uint64_t m = A384020_pillai(k);
    uint64_t sig = 1, tau = 1, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1, e = 0;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
                e++;
            }
            sig *= term;
            tau *= (e + 1);
        }
    }
    if (m > 1) {
        sig *= (m + 1);
        tau *= 2;
    }
    return sig == k * tau;
}

seq_t a(idx_t n)
{
    static uint64_t terms[1 << 14];
    static uint64_t cnt = 0;
    static uint64_t next_k = 1;
    while (cnt < n && cnt < (1 << 14)) {
        if (A384020_test(next_k)) {
            terms[cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
