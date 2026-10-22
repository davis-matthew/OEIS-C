/*
 * A383161  a(n) is the denominator of the mean of the maximum exponents in
 *          the prime factorizations of the unitary divisors of n.
 * https://oeis.org/A383161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator( sum_{d|n, gcd(d,n/d)=1} A051903(d) /
 *            A034444(n) ) where A051903(d) is the maximum exponent in the
 *            prime factorisation of d, and A034444(n) is the number of
 *            unitary divisors of n (2^k, k = #distinct primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator is always a power of two: a(n) = 2^{max(0, k – v2(sum))} where
   k = number of distinct primes of n and v2(sum) is the exponent of 2 in
   sum = Σ_{S⊆[k]} max_{i∈S} e_i. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 1)        /* n = 1 has no prime factors */
        return (seq_t)1;

    /* factorisation: we need only the exponents of the primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    size_t k = cnt;          /* number of distinct primes */

    /* Sort the exponents to group equal values */
    qsort(exps, k, sizeof(uint64_t), cmp_uint64);

    seq_t sum = 0;   /* Σ max exponent over all unitary divisors */

    size_t idx = 0;
    while (idx < k) {
        uint64_t v = exps[idx];        /* current exponent value */
        size_t j = idx;
        while (j < k && exps[j] == v) ++j;
        size_t count_eq = j - idx;
        size_t count_lower = idx;       /* primes with smaller exponent */

        seq_t pow2_lower   = pow2((idx_t)count_lower);       /* 2^{count_lower} */
        seq_t pow2_eq      = pow2((idx_t)count_eq);         /* 2^{count_eq}   */
        seq_t pow2_eq_minus1 = pow2_eq - (seq_t)1;           /* 2^{count_eq} - 1 */

        seq_t base   = pow2_lower * pow2_eq_minus1;          /* 2^{count_lower} * (2^{count_eq} - 1) */
        seq_t term   = (seq_t)v * base;                      /* e * base */

        sum += term;                                         /* accumulate */
        idx = j;
    }

    /* v2(sum) : exponent of 2 in sum */
    size_t v2 = 0;
    while ((sum & (seq_t)1) == 0) {
        sum >>= 1;
        ++v2;
    }

    /* a(n) = 2^{max(0, k – v2)} */
    size_t exp = (k > v2) ? (k - v2) : 0;
    return pow2((idx_t)exp);
}
