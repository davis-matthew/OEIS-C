/*
 * A329036  Number of common divisors of n and A122111(n).
 * https://oeis.org/A329036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(gcd(n, A122111(n))), where A122111(n) is obtained
 *            by conjugating the partition given by the prime indices of n,
 *            i.e. a(n) = Product_{p^e || n} (min(e, f_p) + 1) with f_p the
 *            exponent of p in A122111(n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9970 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool A329036_isprime(uint64_t c)
{
    if (c < 2) return false;
    if (c % 2 == 0) return c == 2;
    for (uint64_t d = 3; d <= c / d; d += 2)
        if (c % d == 0) return false;
    return true;
}

/* index of prime p (p = 2 -> 1) */
static uint64_t A329036_pindex(uint64_t p)
{
    uint64_t cnt = 0;
    for (uint64_t c = 2; c <= p; c++)
        if (A329036_isprime(c)) cnt++;
    return cnt;
}

/* k-th prime, k >= 1 */
static uint64_t A329036_kth_prime(uint64_t k)
{
    uint64_t cnt = 0, c = 1;
    while (cnt < k) {
        c++;
        if (A329036_isprime(c)) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    uint64_t m = (uint64_t)n;
    if (n <= 1) return (seq_t)1;

    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            primes[pcnt] = p; exps[pcnt] = e; pcnt++;
        }
    }
    if (m > 1) { primes[pcnt] = m; exps[pcnt] = 1; pcnt++; }

    /* primes are ascending, so prime indices are ascending too */
    uint64_t pidx[64];
    uint64_t total = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        pidx[i] = A329036_pindex(primes[i]);
        total += exps[i];
    }

    /* A122111(n) = prod over distinct index groups of prime(remaining)^(cur - last + 1) */
    uint64_t rprime[64], rexp[64];
    size_t rc = 0;
    uint64_t remaining = total, last_part = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t cur = pidx[i];
        rprime[rc] = A329036_kth_prime(remaining);
        rexp[rc] = cur - last_part + 1;
        rc++;
        remaining -= exps[i];
        last_part = cur + 1;
    }

    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t mm = 0;
        for (size_t j = 0; j < rc; ++j) {
            if (rprime[j] == primes[i]) {
                mm = rexp[j] < exps[i] ? rexp[j] : exps[i];
                break;
            }
        }
        result *= (seq_t)(mm + 1);
    }
    return result;
}
