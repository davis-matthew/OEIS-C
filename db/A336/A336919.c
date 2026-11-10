/*
 * A336919  Numbers k such that A000005(k) does not divide A003973(k); numbers
 *          k for which A336839(k) > 1.
 * https://oeis.org/A336919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A000005(k) not dividing A003973(k); found by
 *            search. A003973(k) = Product_{p^e || k} (q^(e+1)-1)/(q-1), q =
 *            nextprime(p), tested mod A000005(k)
 * Range:     u8 n <= 42, u16 n <= 7354, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 9612 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A336919_bad(uint64_t k)
{
    uint64_t m = k, tau = 1;
    uint64_t pr[20], ex[20];
    int cnt = 0;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[cnt] = p; ex[cnt] = e; cnt++;
            tau *= (e + 1);
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; cnt++; tau *= 2; }
    if (tau == 1) return false;
    uint64_t res = 1 % tau;
    for (int i = 0; i < cnt; i++) {
        uint64_t q = next_prime(pr[i]) % tau;
        uint64_t sum = 1 % tau, pw = 1 % tau;
        for (uint64_t j = 0; j < ex[i]; j++) {
            pw = (pw * q) % tau;
            sum = (sum + pw) % tau;
        }
        res = (res * sum) % tau;
    }
    return res != 0;
}

seq_t a(idx_t n)
{
    uint64_t count = 0, k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A336919_bad(k)) count++;
    }
    return (seq_t)k;
}
