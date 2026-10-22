/*
 * A346088  Smallest divisor d of n for which A002034(d) = A002034(n), where
 *          A002034(n) is the smallest positive integer k such that k! is a
 *          multiple of n.
 * https://oeis.org/A346088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n for which A002034(d) =
 *            A002034(n), where A002034(m) is the least k such that k! is
 *            divisible by m
 * Range:     u8 n <= 255, u16 n <= 8347, u32 n <= 8347, u64 n <= 8347,
 *            u128 n <= 8347, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Global state used by the recursive search.  The arrays are large enough for
   the factorisation of any 64‑bit unsigned integer. */
static idx_t  g_primes[64];           /* prime factors of the current n */
static uint64_t g_exps[64];            /* their exponents */
static seq_t  g_minm[64][65];          /* minimal m for (prime, exponent) */
static seq_t  g_best;                  /* best divisor found so far */
static seq_t  g_target;                /* A002034(n) of the queried n */
static size_t g_pf;                    /* number of distinct prime factors */

/* Compute the minimal m such that m! is divisible by p^e. */
static seq_t
min_m_for_prime(idx_t p, uint64_t e)
{
    seq_t m = 1;
    while (1) {
        seq_t sum = 0;
        seq_t t   = m;
        while (t) {
            t   /= p;
            sum += t;
        }
        if (sum >= e)
            return m;
        ++m;
    }
}

/* Recursive search over all divisors.  current_d is the divisor built so far
   and current_max is the A002034 value of that divisor. */
static void
dfs(size_t idx, seq_t current_d, seq_t current_max)
{
    if (idx == g_pf) {
        /* Special case: divisor 1 has A002034(1) = 1. */
        seq_t val = (current_d == 1 && current_max == 0) ? (seq_t)1 : current_max;
        if (val == g_target && current_d < g_best)
            g_best = current_d;
        return;
    }

    /* exponent 0 for current prime */
    dfs(idx + 1, current_d, current_max);

    seq_t d = current_d;
    for (uint64_t e = 1; e <= g_exps[idx]; ++e) {
        d = d * g_primes[idx];
        seq_t m = g_minm[idx][e];
        seq_t new_max = (m > current_max) ? m : current_max;
        dfs(idx + 1, d, new_max);
    }
}

/* External entry point.  Implements seq_t a(idx_t n). */
seq_t a(idx_t n)
{
    /* Build the prime factorisation of n. */
    g_pf = 0;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            g_primes[g_pf] = p;
            g_exps[g_pf]   = cnt;
            ++g_pf;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        g_primes[g_pf] = temp;
        g_exps[g_pf]   = 1;
        ++g_pf;
    }

    /* Compute minimal m for each prime power and the target value. */
    if (g_pf == 0) {          /* n == 1 */
        g_target = 1;
    } else {
        g_target = 0;
        for (size_t i = 0; i < g_pf; ++i) {
            for (uint64_t e = 1; e <= g_exps[i]; ++e) {
                g_minm[i][e] = min_m_for_prime(g_primes[i], e);
            }
            if (g_minm[i][g_exps[i]] > g_target)
                g_target = g_minm[i][g_exps[i]];
        }
    }

    /* Start the search: the best divisor is initially n itself. */
    g_best = n;
    dfs(0, (seq_t)1, (seq_t)0);
    return g_best;
}
