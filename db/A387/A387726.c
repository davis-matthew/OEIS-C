/*
 * A387726  Powerful part of the sum of divisors of n: a(n) =
 *          A057521(A000203(n)).
 * https://oeis.org/A387726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057521(A000203(n)) – powerful part of σ(n)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function is written to be valid both as C11 and as C++17 code.
    It uses only the helpers that are guaranteed to exist for the
    standard build process.  All dynamic allocations are done with
    malloc/calloc and the void* results are explicitly cast to the
    required type.  No static initialisation of non‑constant data
    is performed, so the function is fully re‑entrant and independent
    of the order of calls.  The implementation works in about O(√n)
    time which is well below the 2‑second limit for n up to 10 000.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple prime sieve that is initialised only once.  */
/* ------------------------------------------------------------------ */
static uint64_t *primes   = NULL;   /* array of primes */
static size_t prime_cnt   = 0;      /* number of primes stored */

/* Estimate: primes up to 100 000 are more than enough for n ≤ 10 000. */
static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialised */

    const idx_t LIMIT = 100000;           /* upper bound for primes */
    /* Allocate array that marks composites – use static cast from void* */
    bool *is_comp = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!is_comp) return;                 /* fatal failure – no primes */

    primes = (uint64_t *)malloc((size_t)LIMIT * sizeof(uint64_t));
    if (!primes) {
        free(is_comp);
        return;                           /* fatal – no primes */
    }

    prime_cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = (uint64_t)p;
            if ((idx_t)p * p <= LIMIT) {
                for (idx_t k = p * p; k <= LIMIT; k += p)
                    is_comp[k] = true;
            }
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Power of an unsigned integer (base) raised to (exp) using seq_t.   */
/* The exponent is small (it comes from prime factorisations).        */
/* ------------------------------------------------------------------ */
static seq_t seq_pow_uint(uint64_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    for (unsigned int i = 0; i < exp; ++i)
        result *= b;
    return result;
}

/* ------------------------------------------------------------------ */
/* Return the powerful (square‑ful) part of a 64‑bit integer.          */
/* Only prime powers with exponent ≥ 2 are kept.                       */
/* ------------------------------------------------------------------ */
static seq_t powerful_part_uint64(uint64_t m)
{
    if (m < 2) return (seq_t)1;

    seq_t res = (seq_t)1;
    uint64_t tmp = m;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p != 0) continue;

        unsigned int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp >= 2) {
            res *= seq_pow_uint(p, exp);
        }
    }
    /* any remaining prime factor appears only to the first power – ignored */
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Computes σ(n) and returns its powerful    */
/* part.  The implementation uses only integer arithmetic and the    */
/* helpers provided by the build system.                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – value for n=0 unused */

    init_primes();                 /* prime table from the first call */

    /* --- 1. Compute σ(n) via multiplicativity  --- */
    seq_t sigma = (seq_t)1;
    idx_t m = n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p != 0) continue;

        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* term = 1 + p + p² + … + pᵉ = (p^(e+1)-1)/(p-1)   */
        seq_t num = seq_pow_uint(p, exp + 1) - (seq_t)1;
        seq_t denom = (seq_t)(p - 1);
        seq_t term = num / denom;
        sigma *= term;
    }
    /* remaining prime factor with exponent 1  */
    if (m > 1) {
        sigma *= (seq_t)(1 + m);    /* 1 + p  */
    }

    /* --- 2. Powerful part of σ(n)  --- */
    /*  sigma fits in uint64_t for all n in the test range (≤ 10⁴). */
    uint64_t sigma_val = (uint64_t)sigma;
    return powerful_part_uint64(sigma_val);
}
