/*
 * A251718  a(n) = smallest positive integer k such that both A083221(k, n)
 *          and A083221(k+1, n) have at most two prime factors (are primes or
 *          semiprimes).
 * https://oeis.org/A251718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>=1 such that A083221(k,n) and A083221(k+1,n) both
 *            have bigomega <= 2, by search. A083221(k,n) = prime(k)*m, m =
 *            n-th positive integer coprime to the first k-1 primes; it has
 *            <=2 prime factors iff m = 1 or m is prime
 * Range:     u8 n <= 509, u16 n <= 509, u32 n <= 509, u64 n <= 509,
 *            u128 n <= 509, big any n
 * Validated: standard, pass, 136 terms through n = 504 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A251718_NP 3000
static uint32_t A251718_pr[A251718_NP];
static int A251718_init = 0;

static bool A251718_isprime(uint64_t m)
{
    if (m < 2) return false;
    for (uint64_t d = 2; d * d <= m; d++)
        if (m % d == 0) return false;
    return true;
}

static void A251718_setup(void)
{
    int c = 0;
    uint32_t x = 2;
    while (c < A251718_NP) {
        bool ok = true;
        for (int i = 0; i < c && (uint64_t)A251718_pr[i] * A251718_pr[i] <= x; i++)
            if (x % A251718_pr[i] == 0) { ok = false; break; }
        if (ok) A251718_pr[c++] = x;
        x++;
    }
    A251718_init = 1;
}

static bool A251718_good(int k, uint64_t n)
{
    if (n == 1) return true;
    uint64_t count = 0, m = 0;
    while (count < n) {
        m++;
        bool ok = true;
        for (int i = 0; i < k - 1; i++)
            if (m % A251718_pr[i] == 0) { ok = false; break; }
        if (ok) count++;
    }
    return A251718_isprime(m);
}

seq_t a(idx_t n)
{
    if (!A251718_init) A251718_setup();
    int k = 1;
    bool g = A251718_good(1, (uint64_t)n);
    while (k < A251718_NP - 1) {
        bool g2 = A251718_good(k + 1, (uint64_t)n);
        if (g && g2) return (seq_t)k;
        g = g2;
        k++;
    }
    return (seq_t)k;
}
