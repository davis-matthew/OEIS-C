/*
 * A316979  Number of strict factorizations of n into factors > 1 with no
 *          equivalent primes.
 * https://oeis.org/A316979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of strict factorizations of n into distinct factors > 1
   with no pair of equivalent primes. */

/* Inherit the integer types from the build system */
#include <stdint.h>
#include <stdbool.h>

/* Global tables for the prime factorisation of n and the current
   list of factor exponent vectors.  The maximum number of distinct
   primes for n ≤ 10 000 is 5 (2·3·5·7·11 = 2310 < 10 000 < 2·3·5·7·11·13).
   The maximum depth of a factorisation of n ≤ 10 000 is 13
   (2¹³ = 8192).  The limits below are safely larger. */
static uint64_t g_primes[6] = {0};
static int g_primeCount = 0;
static int  g_factor_exp[20][6] = {0};

/* Test whether a complete factorisation (stored in g_factor_exp[0..depth-1])
   contains no pair of equivalent primes. */
static bool valid_factorisation(int depth)
{
    for (int i = 0; i < g_primeCount; ++i) {
        for (int j = i + 1; j < g_primeCount; ++j) {
            bool same = true;
            for (int f = 0; f < depth; ++f) {
                if (g_factor_exp[f][i] != g_factor_exp[f][j]) {
                    same = false;
                    break;
                }
            }
            if (same) return false;           /* equivalent primes found */
        }
    }
    return true;
}

/* Recursively enumerate all strict factorizations of `rem`.
   `min_fact` guarantees that the next factor is strictly larger than
   the previous one, which prevents repetitions of the same factor. */
static void rec(uint64_t rem, uint64_t min_fact, int depth, seq_t *res)
{
    if (rem == 1) {                         /* end of a factorisation */
        if (valid_factorisation(depth)) (*res)++;
        return;
    }

    for (uint64_t f = min_fact; f <= rem; ++f) {
        if (rem % f != 0) continue;          /* skip non‑divisors */

        /* compute the exponent vector of f */
        int exps[6] = {0};
        uint64_t tmp = f;
        for (int pi = 0; pi < g_primeCount; ++pi) {
            uint64_t p = g_primes[pi];
            while (tmp % p == 0) {
                tmp /= p;
                exps[pi]++;
            }
        }

        for (int pi = 0; pi < g_primeCount; ++pi)
            g_factor_exp[depth][pi] = exps[pi];

        rec(rem / f, f + 1, depth + 1, res);  /* next factor must be larger */
    }
}

/* Main sequence routine as required by the harness. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;         /* a(1) = 1 by definition */

    /* Factorise n to obtain its distinct prime divisors. */
    uint64_t tmp = (uint64_t)n;
    g_primeCount = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            g_primes[g_primeCount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) g_primes[g_primeCount++] = tmp;   /* remaining prime factor */

    seq_t result = 0;
    rec((uint64_t)n, 2, 0, &result);
    return result;
}
