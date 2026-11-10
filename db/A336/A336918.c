/*
 * A336918  Numbers k such that A000005(k) divides A003973(k); numbers k for
 *          which A336839(k) = 1.
 * https://oeis.org/A336918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A000005(k) divides A003973(k); found by
 *            search, checking divisibility modulo tau(k)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9135 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A336918_good(uint64_t k)
{
    uint64_t m = k;
    uint64_t tau = 1;
    uint64_t pf[24], ex[24];
    int cnt = 0;
    uint64_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            pf[cnt] = p; ex[cnt] = e; cnt++;
            tau *= (e + 1);
        }
    }
    if (m > 1) { pf[cnt] = m; ex[cnt] = 1; cnt++; tau *= 2; }
    if (tau == 1) return true;
    uint64_t prod = 1 % tau;
    for (int i = 0; i < cnt; i++) {
        uint64_t q = next_prime(pf[i]) % tau;
        uint64_t sum = 1 % tau, pw = 1 % tau;
        for (uint64_t j = 0; j < ex[i]; j++) {
            pw = (uint64_t)((unsigned __int128)pw * q % tau);
            sum = (sum + pw) % tau;
        }
        prod = (uint64_t)((unsigned __int128)prod * sum % tau);
        if (prod == 0) return true;
    }
    return prod == 0;
}

seq_t a(idx_t n)
{
    uint64_t k = 0;
    idx_t count = 0;
    while (count < n) {
        k++;
        if (A336918_good(k)) count++;
    }
    return (seq_t)k;
}
