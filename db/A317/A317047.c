/*
 * A317047  Numbers k such that both k and k + 1 are deficient.
 * https://oeis.org/A317047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 8895, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317047: Numbers k such that both k and k+1 are deficient. */

/* The definition is a property of integers, so the function simply
   tests numbers in order and stores those that satisfy the condition.
   Deficiency is checked by computing the sum of all divisors σ(n)
   (including n) and comparing it to 2n.  A number is deficient iff
   σ(n) < 2n.  The check is performed with trial division (fast for
   the range needed for the tests) and uses the generic seq_t type for
   all arithmetic so it works with 8‑bit, 16‑bit, 32‑bit, 64‑bit,
   or arbitrary‑precision builds.  The function keeps a static array
   of found terms so repeated calls are fast.  The required offset
   of 1 is respected: a(1) is the first term listed. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static storage for the terms (1-indexed). */
static seq_t *terms = NULL;          /* array of terms */
static size_t terms_cap = 0;         /* allocated capacity */
static idx_t term_count = 0;         /* number of terms stored */
static idx_t next_k = 1;             /* next candidate to test */

/* Ensure the array can hold at least 'needed' indices (1‑based). */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Return true if n (idx_t) is a deficient number:
   sum of all divisors σ(n) < 2n. */
static bool is_deficient(idx_t n)
{
    if (n <= 1) return true;        /* 1 is deficient, 0 not used */

    unsigned long long temp = (unsigned long long)n;
    seq_t sigma = 1;                  /* σ(1) = 1 */
    unsigned long long limit = 2ULL * (unsigned long long)n; /* 2n */

    unsigned long long p = 2;
    while (p * p <= temp) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* σ(p^exp) = 1 + p + p^2 + ... + p^exp */
            seq_t term = 1;
            seq_t power = 1;
            for (int e = 0; e < exp; ++e) {
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
            if ((unsigned long long)sigma >= limit) return false;
        }
        ++p;
    }
    if (temp > 1) {                    /* remaining prime factor */
        seq_t term = 1 + (seq_t)temp;
        sigma *= term;
        if ((unsigned long long)sigma >= limit) return false;
    }
    return ((unsigned long long)sigma) < limit;
}

/* Generate terms until we have at least 'upto' terms stored. */
static void generate_terms(idx_t upto)
{
    while (term_count < upto) {
        for (idx_t k = next_k;; ++k) {
            if (is_deficient(k) && is_deficient(k + 1)) {
                ensure_terms_capacity(term_count + 2); /* plus one for safety */
                terms[term_count + 1] = (seq_t)k;     /* 1‑indexed */
                ++term_count;
                next_k = k + 1;                      /* next search starts after k */
                break;
            }
        }
    }
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0ULL;          /* safety for offset < 1 */
    if (n > term_count) generate_terms(n);
    return terms[n];
}
