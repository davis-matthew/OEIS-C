/*
 * A260897  Numerator of det(M) where M is the n X n matrix with M[i,j] =
 *          1/lcm(i,j).
 * https://oeis.org/A260897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^max(∑_{k=1}^n v_2(φ(k)) - 2*v_2(n!), 0) where φ is
 *            Euler's totient and v_2(x) is the exponent of 2 in x
 * Range:     u8 n <= 50, u16 n <= 69, u32 n <= 90, u64 n <= 136,
 *            u128 n <= 204, big any n
 * Validated: standard, pass, 89 terms through n = 500 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static unsigned long long sum_v2_phi(idx_t n)
{
    unsigned long long total = 0;
    /* simple O(n log log n) totient sieve */
    unsigned int *phi = (unsigned int *)malloc((n + 1) * sizeof(unsigned int));
    if (!phi) return 0;
    for (idx_t i = 0; i <= n; ++i) phi[i] = (unsigned int)i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (unsigned int)p) {                /* prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }
    for (idx_t i = 1; i <= n; ++i) {
        unsigned int v = 0;
        unsigned int x = phi[i];
        /* count trailing zeros of x (v_2) */
        while ((x & 1U) == 0) {
            ++v;
            x >>= 1;
        }
        total += v;
    }
    free(phi);
    return total;
}

static unsigned long long v2_factorial(idx_t n)
{
    unsigned long long v = 0;
    for (idx_t p = 2; p <= n; p <<= 1) {
        v += n / p;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 2; /* unused, but keeps the function total order independent */
    unsigned long long total_v2_phi = sum_v2_phi(n);
    unsigned long long v2_fact = v2_factorial(n);
    unsigned long long exp = 0;
    if (total_v2_phi > 2ULL * v2_fact) {
        exp = total_v2_phi - 2ULL * v2_fact;
    }
    /* compute 2^exp */
    seq_t base = 2;
    return ipow(base, (seq_t)exp);
}
