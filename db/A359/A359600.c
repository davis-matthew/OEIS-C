/*
 * A359600  The least odd number with the same prime signature as n.
 * https://oeis.org/A359600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   least odd integer with the same prime signature as n a(n) =
 *            product_{i=1..k} p_i ^ e_i where e_1 ≥ e_2 ≥ … ≥ e_k are the
 *            exponents of n sorted descending and p_1,p_2,… are the odd
 *            primes 3,5,7,…
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* first 30 odd primes – enough for all n up to 10 000 */
static const uint32_t odd_primes[30] = {
    3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97,101,103,107,109,113,127
};

/* simple integer power for seq_t (works for built‑in types and
   for arbitrary‑precision integer types that overload *) */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* collect the exponents of all prime factors of n */
    idx_t exps[64];
    size_t k = 0;
    idx_t m = n;

    /* factor out 2 first (fast) */
    idx_t e = 0;
    while ((m & 1) == 0) {
        ++e;
        m >>= 1;
    }
    if (e)
        exps[k++] = e;

    /* trial division for odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[k++] = e;
        }
    }

    /* leftover prime factor > sqrt(original n) */
    if (m > 1)
        exps[k++] = 1;

    /* sort exponents descending */
    for (size_t i = 0; i < k; ++i)
        for (size_t j = i + 1; j < k; ++j)
            if (exps[i] < exps[j]) {
                idx_t t = exps[i]; exps[i] = exps[j]; exps[j] = t;
            }

    /* build the minimal odd number */
    seq_t result = 1;
    for (size_t i = 0; i < k; ++i) {
        seq_t term = seq_pow_uint((seq_t)odd_primes[i], exps[i]);
        result *= term;
    }
    return result;
}
