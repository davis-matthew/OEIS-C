/*
 * A328098  If A327928(n) is zero, a(n) = A003415(n), otherwise a(n) =
 *          A327858(n) = gcd(A003415(n), A276086(n)).
 * https://oeis.org/A328098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) if A327928(n) = 0, otherwise a(n) =
 *            gcd(A003415(n), A276086(A003415(n))), where A003415 is the
 *            arithmetic derivative and A276086 is the primorial base
 *            exp-function. For the gcd, each prime p contributes
 *            p^min(v_p(n'), d_p), where d_p is the digit of n' at position p
 *            in primorial...
 * Range:     u8 n <= 233, u16 n <= 38249, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9695 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A328098_deriv(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t res = 0, rem = n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        uint64_t e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e > 0) res += (n / p) * e;
    }
    if (rem > 1) res += n / rem;
    return res;
}

/* number of distinct primes p with p^p | d */
static int A328098_count(uint64_t d)
{
    int cnt = 0;
    if (d <= 1) return 0;
    uint64_t rem = d;
    for (uint64_t p = 2; p * p <= rem; p++) {
        uint64_t e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e >= p) cnt++;
    }
    return cnt;
}

static bool A328098_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t q = 2; q * q <= x; q++)
        if (x % q == 0) return false;
    return true;
}

/* gcd(d, A276086(d)) without forming A276086(d) */
static uint64_t A328098_gcd_prim(uint64_t d)
{
    uint64_t result = 1, m = d, p = 2;
    while (m > 0) {
        uint64_t digit = m % p;
        m /= p;
        if (digit > 0) {
            uint64_t t = d;
            uint64_t v = 0;
            while (v < digit && t % p == 0) { t /= p; v++; result *= p; }
        }
        do { p++; } while (!A328098_isprime(p));
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t d = A328098_deriv((uint64_t)n);
    if (A328098_count(d) == 0) return (seq_t)d;
    return (seq_t)A328098_gcd_prim(d);
}
