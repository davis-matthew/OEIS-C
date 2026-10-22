/*
 * A396586  a(n) / (6*n!) is the quadratic moment of the Farey series of order
 *          n.
 * https://oeis.org/A396586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n! * (3 + Σ_{m=1..n} (2*φ(m) + J_{-1}(m))) 
 * where φ is Euler's totient and J_{-1}(m) = m^{-1} * Π_{p|m}(1-p).
 * This is equivalent to a(n) = 6*n! * Σ_{r∈F_n} r^2.
 * The implementation precomputes φ(m) and the product Π_{p|m}(1-p) for all
 * m ≤ n using a sieve of smallest prime factors.  All arithmetic is done
 * with seq_t; any negative intermediate value is handled by subtracting a
 * positive product when required.  Memoisation is used so repeated calls
 * reuse previously computed arrays up to the largest n seen.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factorial.h"

/* static cache for sieve up to the largest n seen */
static idx_t cached_max_n = 0;
static uint32_t *spf = NULL;        /* smallest prime factor */
static int *phi_arr = NULL;         /* Euler's totient */
static int64_t *prod_arr = NULL;    /* Π_{p|m}(1-p) product, signed */

/* Ensure sieve and value arrays up to n */
static void ensure_sieve(idx_t n)
{
    if (n <= cached_max_n) return;

    /* Reallocate or allocate arrays */
    spf = (uint32_t *)realloc(spf, (n + 1) * sizeof(uint32_t));
    phi_arr = (int *)realloc(phi_arr, (n + 1) * sizeof(int));
    prod_arr = (int64_t *)realloc(prod_arr, (n + 1) * sizeof(int64_t));
    if (!spf || !phi_arr || !prod_arr) abort();

    /* initialise spf to itself */
    for (idx_t i = 0; i <= n; ++i) spf[i] = (uint32_t)i;
    spf[0] = spf[1] = 0;

    /* sieve smallest prime factors */
    for (idx_t i = 2; (uint64_t)i * i <= n; ++i) {
        if (spf[i] == (uint32_t)i) { /* i is prime */
            for (uint64_t j = (uint64_t)i * i; j <= n; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* compute φ and product Π (1-p) for each m */
    phi_arr[0] = 0;
    phi_arr[1] = 1;
    prod_arr[1] = 1;
    for (idx_t m = 2; m <= n; ++m) {
        /* φ(m) using spf */
        uint32_t p = spf[m];
        idx_t m_div_p = m / p;
        if (m_div_p % p == 0) {
            phi_arr[m] = phi_arr[m_div_p] * (int)p;
        } else {
            phi_arr[m] = phi_arr[m_div_p] * ((int)p - 1);
        }

        /* product Π_{p|m}(1-p) */
        int64_t prod = 1;
        idx_t t = m;
        int64_t prev = -1;
        while (t > 1) {
            int64_t cur = spf[t];
            if (cur != prev) {
                prod *= (int64_t)(1 - cur);
                prev = cur;
            }
            while (t % cur == 0) t /= cur;
        }
        prod_arr[m] = prod;
    }

    cached_max_n = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1 */

    ensure_sieve(n);

    seq_t fact = factorial(n);      /* n! */
    seq_t result = 3 * fact;        /* term 3*n! */

    for (idx_t m = 1; m <= n; ++m) {
        /* add n! * 2*phi(m) */
        seq_t term1 = fact * (seq_t)(2 * phi_arr[m]);
        result += term1;

        /* add (n!/m) * Π(1-p) */
        seq_t part = fact / (seq_t)m;      /* integer division, exact */
        int64_t prod = prod_arr[m];
        seq_t abs_prod = (seq_t)(prod >= 0 ? prod : -prod);
        seq_t term2 = part * abs_prod;
        if (prod >= 0)
            result += term2;
        else
            result -= term2;
    }

    return result;
}
