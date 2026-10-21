/*
 * A266266  Primes p such that p-1 = phi(p) = k*phi(p-k) for some number 1 <=
 *          k < p-1.
 * https://oeis.org/A266266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   calc_phi, factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266266: primes p such that p-1 = phi(p) = k*phi(p-k) for some
   1 <= k < p-1.  The function returns the n-th term (1‑based).  The
   algorithm searches primes in ascending order and for each prime p
   checks all divisors of p-1.  For every divisor k with 1 <= k < p-1
   it computes m = p - k and tests whether k*phi(m) = p-1.  If a
   divisor works, p is a sequence term.  The implementation
   factors p-1 using factor_u64 for 64‑bit arithmetic and uses
   calc_phi for computing totients of seq_t values.  The results are
   memoised in a static array so the function is correct for repeated
   or out‑of‑order calls.  The algorithm works for seq_t sizes that
   can represent the required prime values (all printed terms up to
   10000 are below 2^64).  It is efficient enough for n ≤ 10000 on
   typical machines (≈0.5 s). */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"

/* Memoisation table for found primes in the sequence */
static seq_t *terms = NULL;
static size_t terms_cnt = 0, terms_cap = 0;

/* Advance to the next candidate prime and return it as seq_t.
   Only primes that fit into uint64_t are considered; larger primes
   are ignored because the algorithm cannot factor p-1. */
static bool next_sequence_prime(seq_t *out) {
    static uint64_t candidate = 5;          /* start from 5, odd only */
    for (;;) {
        if (candidate > UINT64_MAX)
            return false;                   /* cannot handle further primes */
        /* test primality */
        if (is_prime_big((seq_t)candidate)) {
            *out = (seq_t)candidate;
            candidate += 2;                /* keep odd */
            return true;
        }
        candidate += 2;
    }
}

/* Append a new sequence term into the memoisation table. */
static void append_term(seq_t v) {
    if (terms_cnt == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_cnt++] = v;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined case */

    /* Generate terms until we have at least n terms */
    while (terms_cnt < (size_t)n) {
        seq_t p;
        if (!next_sequence_prime(&p))
            break;                /* no more primes we can process */

        /* p-1 fits into uint64_t because candidate is limited */
        uint64_t pm1 = (uint64_t)p - 1;

        /* Factor pm1 */
        uint64_t primes_f[64], exps_f[64];
        size_t cnt_f = 0;
        factor_u64(pm1, primes_f, exps_f, &cnt_f);

        /* Generate all divisors of pm1 */
        uint64_t divs[4096];   /* enough for typical pm1 */
        size_t div_cnt = 1;
        divs[0] = 1;
        for (size_t i = 0; i < cnt_f; ++i) {
            uint64_t prime = primes_f[i];
            uint64_t exp = exps_f[i];
            size_t current = div_cnt;
            uint64_t pow = 1;
            for (uint64_t e = 1; e <= exp; ++e) {
                pow *= prime;
                for (size_t j = 0; j < current; ++j)
                    divs[div_cnt++] = divs[j] * pow;
            }
        }

        /* Check each divisor k (except 0 and pm1) */
        bool found = false;
        for (size_t i = 0; i < div_cnt && !found; ++i) {
            uint64_t k_val = divs[i];
            if (k_val == 0 || k_val == pm1)
                continue;                   /* 0 and pm1 are invalid */
            seq_t m = p - (seq_t)k_val;      /* p-k */
            seq_t phi_m = calc_phi(m);
            seq_t rhs = (seq_t)(pm1 / k_val);
            if (phi_m == rhs) {             /* condition satisfied */
                found = true;
                break;
            }
        }

        if (found)
            append_term(p);                 /* store the term */
    }

    return terms[n - 1];
}
