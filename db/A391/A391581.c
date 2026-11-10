/*
 * A391581  Numbers k whose binary expansion contains 2 adjacent 1's and
 *          A391571(k) = k.
 * https://oeis.org/A391581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term iff k has two adjacent 1 bits (k & (k>>1) !=
 *            0) and A391571(k) = k, i.e. for the smallest odd m > 1 for which
 *            k*m is exactly divisible by m in carryless base-2 (GF(2)[x])
 *            division, the quotient is k. All arithmetic is done in native
 *            64-bit integers; results converted to seq_t
 * Range:     u8 none, u16 n <= 132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 8690 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int A391581_bl(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* carryless division over GF(2); returns quotient if exact, else 0 */
static uint64_t A391581_cldiv(uint64_t dividend, uint64_t divisor, int div_deg)
{
    uint64_t rem = dividend, quo = 0;
    while (rem != 0) {
        int rd = A391581_bl(rem) - 1;
        if (rd < div_deg)
            return 0;
        int s = rd - div_deg;
        quo |= (uint64_t)1 << s;
        rem ^= divisor << s;
    }
    return quo;
}

/* A391571(n), computed natively */
static uint64_t A391581_A391571(uint64_t n)
{
    for (uint64_t k = 3; ; k += 2) {
        uint64_t prod = n * k;
        uint64_t q = A391581_cldiv(prod, k, A391581_bl(k) - 1);
        if (q != 0)
            return q;
    }
}

static uint64_t *g_terms = 0;
static size_t g_count = 0;
static size_t g_cap = 0;
static uint64_t g_next = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (g_count < (size_t)n) {
        uint64_t m = g_next++;
        if ((m & (m >> 1)) == 0)
            continue;
        if (A391581_A391571(m) == m) {
            if (g_count == g_cap) {
                size_t nc = g_cap ? g_cap * 2 : 256;
                g_terms = (uint64_t *)realloc(g_terms, nc * sizeof(uint64_t));
                g_cap = nc;
            }
            g_terms[g_count++] = m;
        }
    }
    return (seq_t)g_terms[n - 1];
}
