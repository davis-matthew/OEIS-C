/*
 * A348961  Exponential harmonic (or e-harmonic) numbers of type 1: numbers k
 *          such that esigma(k) | k * d_e(k), where d_e(k) is the number of
 *          exponential divisors of k (A049419) and esigma(k) is their sum
 *          (A051377).
 * https://oeis.org/A348961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 8718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth e‑harmonic number of type 1 (OEIS A348961) by
   testing integers sequentially and checking the condition
   esigma(k) | k · d_e(k). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_uint32.h"

/* fast exponentiation for seq_t; exponent fits in idx_t (usually tiny) */
static seq_t pow_seq_uint64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Smallest prime factor table */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Build spf up to at least the given limit */
static void compute_spf(idx_t limit)
{
    if (spf != NULL && limit <= spf_limit) return;
    if (spf != NULL) free(spf);

    spf_limit = limit;
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    if (limit >= 1) spf[1] = 1;
}

/* Test whether k satisfies esigma(k) | k·d_e(k) */
static bool is_e_harmonic(idx_t k)
{
    if (k == 1) return true;

    if (k > spf_limit) compute_spf(k * 2);

    idx_t tmp = k;
    idx_t primes[32];
    unsigned exps[32];
    size_t pc = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        unsigned e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        if (pc < 32) {
            primes[pc] = p;
            exps[pc] = e;
            pc++;
        } else {
            return false; /* safety: should not happen for our limits */
        }
    }

    seq_t esigma = 1;
    seq_t d_e_prod = 1;

    for (size_t i = 0; i < pc; ++i) {
        unsigned e = exps[i];
        /* d(e) */
        unsigned de_i = divisor_count_uint32(e);
        d_e_prod *= (seq_t)de_i;

        /* sum of p^f over divisors f of e */
        seq_t sum_pi_f = 0;
        for (unsigned f = 1; f <= e; ++f)
            if (e % f == 0)
                sum_pi_f += pow_seq_uint64((seq_t)primes[i], f);
        esigma *= sum_pi_f;
    }

    seq_t rhs = (seq_t)k * d_e_prod;
    return (rhs % esigma == 0);
}

/* Storage for the sequence values */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;
static idx_t next_candidate = 1;

/* Ensure capacity for terms array */
static void grow_terms(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) exit(1);
    terms = tmp;
    terms_cap = new_cap;
}

/* Append a new term to the sequence */
static void append_term(seq_t value)
{
    grow_terms(terms_cnt + 1);
    terms[terms_cnt++] = value;
}

/* Required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, guard clause */
    while (terms_cnt < (size_t)n) {
        if (is_e_harmonic(next_candidate))
            append_term(next_candidate);
        ++next_candidate;
    }
    return terms[n - 1];
}
