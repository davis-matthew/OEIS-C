/*
 * A348427  Composite k for which sigma(k) is divisible by the sum of the
 *          arithmetic derivatives of the divisors of k.
 * https://oeis.org/A348427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   factor_u64, is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A348427: composite k for which σ(k) is divisible by the sum
 *   of the arithmetic derivatives of the divisors of k.
 *  Formula: a(n) = the n‑th composite integer k such that
 *   σ(k) mod ( Σ_{d|k} d' ) == 0,
 *   where d' = d * Σ_{p|d} v_p(d)/p.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

/* ---------- static cache of found terms ---------- */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;

/* add a new term to the cache – grows the array if needed */
static void append_term(uint64_t val)
{
    if (terms_cap <= terms_size) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_size++] = (seq_t)val;
}

/* compute sigma(k) and the sum of arithmetic derivatives of its divisors */
static void sigma_and_derivative_sum(uint64_t k, seq_t *sigma, seq_t *der_sum)
{
    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);

    seq_t sigma_val = 1;
    seq_t A[32];      /* Σ_{f=0}^{e_i} p_i^f       */
    seq_t B[32];      /* Σ_{f=1}^{e_i} f·p_i^f     */

    /* first pass: compute A_i, B_i and σ = ∏ A_i */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = pr[i];
        uint64_t e  = ex[i];

        /* A_i = (p^{e+1} - 1)/(p-1) */
        seq_t p_pow_e1 = pow_uint(p, e + 1);
        seq_t A_i = (p_pow_e1 - 1) / ((seq_t)p - 1);
        A[i] = A_i;

        /* B_i = Σ_{f=1}^e f·p^f */
        seq_t B_i = 0;
        seq_t p_pow = 1;   /* starts with p^0 */
        for (uint64_t f = 1; f <= e; ++f) {
            p_pow *= p;
            B_i += (seq_t)f * p_pow;
        }
        B[i] = B_i;

        sigma_val *= A_i;
    }

    /* second pass: compute derivative sum */
    seq_t g = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* term_i = B_i * (σ / A_i) / p_i */
        seq_t sigma_div_Ai = sigma_val / A[i];          /* exact division */
        seq_t mult = sigma_div_Ai * B[i];
        seq_t term = mult / pr[i];                      /* exact division */
        g += term;
    }

    *sigma  = sigma_val;
    *der_sum = g;
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 – safety guard */

    /* maintain static state of the next candidate to test */
    static uint64_t current = 4;     /* smallest composite */

    /* generate terms until we have at least n of them */
    while (terms_size < (size_t)n) {
        /* skip primes and numbers < 4 */
        while (is_prime((idx_t)current) || current < 4) current++;

        seq_t sigma, g;
        sigma_and_derivative_sum(current, &sigma, &g);

        if (g != 0 && sigma % g == 0) {
            append_term(current);
            current++;                 /* next search starts after this term */
        } else {
            current++;                 /* continue scanning */
        }
    }

    return terms[n - 1];
}
