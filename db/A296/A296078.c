/*
 * A296078  Least number with the same prime signature as 1+phi(n), where phi
 *          = A000010, Euler totient function.
 * https://oeis.org/A296078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2664, u16 n <= 9627, u32 n <= 9627, u64 n <= 9627,
 *            u128 n <= 9627, big any n
 * Helpers:   build_primes, phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = least number having the same prime signature as
    1 + φ(n)  (Euler's totient).  The prime signature is the
    multiset of exponents in the prime factorization.  For a
    given signature  e₁ ≥ e₂ ≥ … ≥ e_k, the minimal number is obtained
    by assigning the smallest primes to the largest exponents. */

/*  External helpers available in the build environment  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/phi.h"            /* seq_t phi(idx_t n); */
#include "../utils/pow_uint.h"       /* seq_t pow_uint(uint64_t base, idx_t exp); */

static bool initialized = false;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

/* comparator to sort exponents in descending order (largest first) */
static int cmp_desc(const void *a, const void *b)
{
    unsigned int x = *(const unsigned int *)a;
    unsigned int y = *(const unsigned int *)b;
    return (x < y) - (x > y);   /* >0 if x<y, <0 if x>y */
}

seq_t a(idx_t n)
{
    /* initialise one‑time */
    if (!initialized) {
        /* enough to cover sqrt(1+φ(n)) for n ≤ 10 000 and far beyond */
        build_primes(200, &prime_list, &prime_count);
        initialized = true;
    }

    /* m = 1 + φ(n) */
    seq_t phi_n = phi(n);
    seq_t m = phi_n + (seq_t)1;    /* 1 + φ(n) */

    /* factor m using the pre‑computed primes */
    unsigned int exps[64];
    size_t ek = 0;
    uint64_t tmp = (uint64_t)m;    /* safe: m ≤ n+1 ≤ 10 001 */

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        unsigned int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e) exps[ek++] = e;
    }
    if (tmp > 1)      /* remaining prime factor */
        exps[ek++] = 1;

    /* sort exponents descending (largest exponent gets smallest prime) */
    if (ek > 1)
        qsort(exps, ek, sizeof(unsigned int), cmp_desc);

    /* build the minimal number with this signature */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < ek; ++i) {
        result = result * pow_uint((uint64_t)prime_list[i], exps[i]);
    }
    return result;
}
