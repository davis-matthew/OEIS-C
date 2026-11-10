/*
 * A364259  a(n) = A331410(A332212(n)).
 * https://oeis.org/A364259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410(A332212(n)), fully additive. A332212 is fully
 *            multiplicative with A332212(prime(k)) = 2^k - 1 when k is a
 *            Mersenne exponent, and otherwise the j-th prime that is not a
 *            Mersenne prime, where j = k - (number of Mersenne exponents <=
 *            k). A331410 is fully additive: A331410(2) = 0, A331...
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9755 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const uint64_t A364259_mexp[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521, 607, 1279, 2203, 2281,
    3217, 4253, 4423, 9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243,
    110503, 132049, 216091, 756839, 859433, 1257787, 1398269, 2976221,
    3021377, 6972593, 13466917, 20996011, 24036583, 25964951, 30402457,
    32582657, 37156667, 42643801, 43112609, 57885161, 74207281, 77232917,
    82589933, 136279841
};

static unsigned char *A364259_comp = NULL; /* 1 = composite or < 2 */
static uint64_t A364259_lim = 0;

static void A364259_ensure(uint64_t lim)
{
    uint64_t nl, i, j;
    if (lim <= A364259_lim)
        return;
    nl = A364259_lim * 2;
    if (nl < lim)
        nl = lim;
    if (nl < 1024)
        nl = 1024;
    if (A364259_comp)
        free(A364259_comp);
    A364259_comp = (unsigned char *)calloc((size_t)(nl + 1), 1);
    A364259_comp[0] = 1;
    A364259_comp[1] = 1;
    for (i = 2; i * i <= nl; i++)
        if (!A364259_comp[i])
            for (j = i * i; j <= nl; j += i)
                A364259_comp[j] = 1;
    A364259_lim = nl;
}

static bool A364259_isp(uint64_t q)
{
    A364259_ensure(q);
    return !A364259_comp[q];
}

static uint64_t A364259_pi(uint64_t p)
{
    uint64_t c = 0, q;
    A364259_ensure(p);
    for (q = 2; q <= p; q++)
        if (!A364259_comp[q])
            c++;
    return c;
}

/* A331410 over uint64, fully additive */
static uint64_t A364259_A331410(uint64_t x)
{
    uint64_t total = 0, p;
    while (x % 2 == 0 && x > 0)
        x /= 2;
    for (p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                e++;
            }
            total += e * (1 + A364259_A331410(p + 1));
        }
    }
    if (x > 1)
        total += 1 + A364259_A331410(x + 1);
    return total;
}

/* A331410(A332212(prime(k))) */
static uint64_t A364259_prime_term(uint64_t k)
{
    uint64_t m = 0, j, q, cnt;
    size_t i, nm = sizeof(A364259_mexp) / sizeof(A364259_mexp[0]);
    for (i = 0; i < nm; i++) {
        if (A364259_mexp[i] == k)
            return 1;
        if (A364259_mexp[i] < k)
            m++;
    }
    j = k - m;
    cnt = 0;
    for (q = 2;; q++) {
        if (A364259_isp(q) && ((q + 1) & q) != 0) {
            cnt++;
            if (cnt == j)
                return A364259_A331410(q);
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t total = 0, p;
    if (m <= 1)
        return (seq_t)0;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            total += e * A364259_prime_term(A364259_pi(p));
        }
    }
    if (m > 1)
        total += A364259_prime_term(A364259_pi(m));
    return (seq_t)total;
}
