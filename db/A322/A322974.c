/*
 * A322974  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          f(i) = f(j) for all i, j, where f(1) = 0, f(2) = 1, f(n) = 3 if
 *          A009191(n) == 1 and f(n) = A049820(n) for all other numbers > 2.
 * https://oeis.org/A322974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1..n), where
 *            f(1)=0, f(2)=1, f(n)=3 if gcd(n,tau(n))=1, else n-tau(n);
 *            computed by direct scan (restricted growth labeling)
 * Range:     u8 n <= 673, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9261 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A322974_tau(uint64_t m)
{
    uint64_t c = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            c *= (e + 1);
        }
    }
    if (m > 1) c *= 2;
    return c;
}

static uint64_t A322974_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A322974_f(uint64_t k)
{
    if (k == 1) return 0;
    if (k == 2) return 1;
    uint64_t d = A322974_tau(k);
    if (A322974_gcd(k, d) == 1) return 3;
    return k - d;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N = (uint64_t)n;
    uint64_t *label = (uint64_t *)calloc((size_t)N + 2, sizeof(uint64_t));
    if (!label) return 0;
    uint64_t count = 0, res = 0;
    for (uint64_t k = 1; k <= N; ++k) {
        uint64_t v = A322974_f(k);
        if (label[v] == 0) label[v] = ++count;
        if (k == N) res = label[v];
    }
    free(label);
    return (seq_t)res;
}
