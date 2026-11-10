/*
 * A319694  Product of A078899(x) for all x encountered when map x ->
 *          A252463(x) is iterated, starting from x = A252463(n); a(1) = 1.
 * https://oeis.org/A319694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{x in orbit} A078899(x), orbit = x0=A252463(n),
 *            x_{i+1}=A252463(x_i), ending at x=1 (included); a(1)=1. Computed
 *            by a gpf sieve plus direct iteration
 * Range:     u8 n <= 95, u16 n <= 360, u32 n <= 960, u64 n <= 3480,
 *            u128 n <= 11448, big any n
 * Validated: standard, pass, 121 terms through n = 9859 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool a319694_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m < 4) return true;
    if ((m & 1u) == 0) return false;
    for (uint64_t d = 3; d <= m / d; d += 2)
        if (m % d == 0) return false;
    return true;
}

static uint64_t a319694_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!a319694_isprime(q)) --q;
    return q;
}

/* A252463 on native integers (values never exceed the argument) */
static uint64_t a319694_A252463(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0) return n >> 1;
    uint64_t m = n, result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t base = a319694_prevprime(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= a319694_prevprime(m);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return (seq_t)1;

    size_t lim = (size_t)n;
    uint32_t *gpf = (uint32_t *)calloc(lim + 1, sizeof(uint32_t));
    if (!gpf) return 0;
    for (size_t p = 2; p <= lim; ++p) {
        if (gpf[p] == 0) {
            for (size_t m = p; m <= lim; m += p) gpf[m] = (uint32_t)p;
        }
    }
    gpf[1] = 1;

    seq_t result = (seq_t)1;
    uint64_t x = a319694_A252463((uint64_t)n);
    for (;;) {
        uint32_t g = gpf[x];
        uint64_t cnt = 0;
        for (uint64_t k = 1; k <= x; ++k)
            if (gpf[k] == g) ++cnt;
        result = result * (seq_t)cnt;
        if (x == 1) break;
        x = a319694_A252463(x);
    }
    free(gpf);
    return result;
}
