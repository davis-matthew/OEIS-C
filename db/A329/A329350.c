/*
 * A329350  a(n) = Product_{d|n} A276086(d)^A010051(n/d).
 * https://oeis.org/A329350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p prime, p|n} A276086(n/p), where A276086(m) =
 *            Product_k prime(k+1)^d_k and d_k are the primorial-base digits
 *            of m
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 139, u64 n <= 769,
 *            u128 n <= 3761, big any n
 * Validated: standard, pass, 88 terms through n = 7073 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A329350_kth_prime(int k) /* k>=1 */
{
    uint64_t c = 1;
    int cnt = 0;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t q = 2; q * q <= c; q++)
            if (c % q == 0) { ok = false; break; }
        if (ok) cnt++;
    }
    return c;
}

static seq_t A329350_A276086(idx_t m)
{
    seq_t res = 1;
    uint64_t x = (uint64_t)m;
    int k = 1;
    while (x > 0) {
        uint64_t p = A329350_kth_prime(k);
        uint64_t digit = x % p;
        x /= p;
        for (uint64_t i = 0; i < digit; i++)
            res = res * (seq_t)p;
        k++;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            result = result * A329350_A276086(n / p);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1)
        result = result * A329350_A276086(n / m);
    return result;
}
