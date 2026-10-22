/*
 * A333926  The sum of recursive divisors of n.
 * https://oeis.org/A333926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^k of (1 + Σ_{d recursive
 *            divisor of k} p^d)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

#define MAX_EXP 70           /* enough for exponents up to 64‑bit numbers (≤ 2^63) */

/* marks[k][d] == true  ⇔  d is a recursive divisor of k   (1 ≤ d ≤ k) */
static bool marks[MAX_EXP + 1][MAX_EXP + 1];
static bool computed[MAX_EXP + 1];

/* recursive computation of the marks table up to k */
static void compute_R(idx_t k)
{
    if (k > MAX_EXP) return;          /* safety: all needed k will be ≤ MAX_EXP */
    if (computed[k]) return;

    for (idx_t i = 1; i < k; ++i) compute_R(i);

    /* initialise current row */
    for (idx_t d = 1; d <= k; ++d) marks[k][d] = false;

    /* inherit divisors from all smaller exponents */
    for (idx_t i = 1; i < k; ++i) {
        for (idx_t d = 1; d <= i; ++d) {
            if (marks[i][d] && (k % d == 0))
                marks[k][d] = true;
        }
    }

    /* k itself is always a recursive divisor of k */
    marks[k][k] = true;
    computed[k] = true;
}

/* evaluate 1 + Σ_{d recursive divisor of e} p^d   for a prime power p^e */
static seq_t prime_power_term(uint64_t p, idx_t e)
{
    compute_R(e);
    seq_t sum = 1;           /* the leading 1 in the formula */
    seq_t pd;

    for (idx_t d = 1; d <= e; ++d) {
        if (marks[e][d]) {
            pd = seq_pow((seq_t)p, d);   /* given in ../utils/seq_pow.h */
            sum += pd;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;   /* a(1) = 1 */

    /* factorisation of n */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t   num;

    factor_u64((uint64_t)n, primes, exponents, &num);

    seq_t result = 1;
    for (size_t i = 0; i < num; ++i) {
        seq_t term = prime_power_term(primes[i], (idx_t)exponents[i]);
        result *= term;
    }
    return result;
}
