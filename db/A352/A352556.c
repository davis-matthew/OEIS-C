/*
 * A352556  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 24.
 * https://oeis.org/A352556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of modules of size n over the ring of integers of
 *            the quadratic field with discriminant 24. Implementation by
 *            factorising n, determining the splitting type of each prime, and
 *            multiplying the appropriate partition/convolution counts
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre24, pow_mod_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/legendre24.h"
#include "../utils/pow_mod_ull.h"

#define MAX_EXP 20
static seq_t part[MAX_EXP + 1];
static seq_t conv[MAX_EXP + 1];
static int initialized = 0;

static void init_partitions(void)
{
    if (initialized) return;
    part[0] = 1;
    for (idx_t i = 1; i <= MAX_EXP; ++i) part[i] = 0;
    for (idx_t i = 1; i <= MAX_EXP; ++i)
        for (idx_t j = i; j <= MAX_EXP; ++j)
            part[j] += part[j - i];      /* partitions */
    for (idx_t e = 0; e <= MAX_EXP; ++e) {
        seq_t sum = 0;
        for (idx_t a = 0; a <= e; ++a)
            sum += part[a] * part[e - a]; /* convolution for split primes */
        conv[e] = sum;
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_partitions();

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }

        seq_t contrib;
        if (p == 2 || p == 3) {                 /* ramified primes */
            contrib = part[e];
        } else {
            int ls = legendre24((int)p);
            if (ls == 1) {                      /* split */
                contrib = conv[e];
            } else {                            /* inert */
                if (e & 1) return 0;
                contrib = part[e >> 1];
            }
        }
        result *= contrib;
        if (result == 0) return 0;
    }

    if (m > 1) {                                /* remaining prime factor */
        idx_t p = m, e = 1;
        seq_t contrib;
        if (p == 2 || p == 3) {
            contrib = part[e];
        } else {
            int ls = legendre24((int)p);
            if (ls == 1) {
                contrib = conv[e];
            } else {
                if (e & 1) return 0;
                contrib = part[e >> 1];
            }
        }
        result *= contrib;
    }

    return result;
}
