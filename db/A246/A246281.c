/*
 * A246281  Numbers k for which A003961(k) < 2*k; Numbers n such that if n =
 *          product_{k >= 1} (p_k)^(c_k), then product_{k >= 1}
 *          (p_{k+1})^(c_k) < 2*n, where p_k indicates the k-th prime,
 *          A000040(k).
 * https://oeis.org/A246281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246281 – Numbers k for which A003961(k) < 2*k.
   We generate the increasing list by testing every integer n
   in order.  For each n we factor it using a simple sieve of
   smallest prime factors; the product of the successor primes
   raised to the same exponents is compared with 2*n.
   The n‑th term a(n) is then returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
/* utilities provided by the build system */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t start); */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

/* --- static storage for the sequence ----------------------------- */
static seq_t *seq_values = NULL;   /* a(1) stored at seq_values[0] */
static size_t seq_len   = 0;       /* number of values stored */
static size_t seq_cap   = 0;       /* allocated capacity */

/* --- sieve for smallest prime factors --------------------------- */
static uint32_t *spf       = NULL; /* spf[n] = smallest prime factor of n */
static size_t    spf_limit = 0;    /* current maximum index filled in spf */

/* ensure spf table is built up to at least `limit` (inclusive) */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    /* free old table and allocate new */
    free(spf);
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) abort();

    for (uint32_t i = 2; i <= limit; ++i)
    {
        if (!spf[i])            /* i is prime */
        {
            spf[i] = i;
            if ((uint64_t)i * i <= limit)
            {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    spf_limit = limit;
}

/* test whether n satisfies the inequality */
static bool satisfies(idx_t n)
{
    if (n == 1) return true;          /* empty product: 1 < 2 */
    seq_t N = n;                      /* original number         */
    seq_t S = 1;                      /* product of next primes  */
    int tmp = (int)n;                 /* mutable copy for factoring */

    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        int exp = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp;
        }

        uint64_t nextP = next_prime(p);                 /* successor prime */
        seq_t pow_val = pow_uint((uint64_t)nextP, (idx_t)exp);
        S *= pow_val;

        /* early exit if already exceeding 2*N */
        seq_t limit = N + N;
        if (S >= limit) return false;
    }
    return S < N + N;
}

/* append a new term to the sequence array */
static void push_seq(seq_t term)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_values = tmp;
        seq_cap = new_cap;
    }
    seq_values[seq_len++] = term;
}

/* --------------------------------------------------------------- */
/* public interface */
seq_t a(idx_t n)
{
    /* guard for accidental zero (offset is 1) */
    if (n == 0) return 0;

    /* static progress state */
    static idx_t current_scan = 1;   /* next integer to test */

    /* generate terms until we have at least `n` of them */
    while (seq_len < n)
    {
        if ((size_t)current_scan > spf_limit)
            ensure_spf((spf_limit == 0) ? 1024 : spf_limit * 2);

        if (satisfies(current_scan))
            push_seq(current_scan);

        ++current_scan;
    }

    return seq_values[n - 1];
}
