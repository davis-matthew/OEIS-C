/*
 * A347124  Möbius transform of A341528, n * sigma(A003961(n)).
 * https://oeis.org/A347124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2590, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A347124: Möbius transform of A341528, n * sigma(A003961(n)).
   The sequence is multiplicative.
   a(p^e) = p^(e-1) * (q^e*(p*q-1)-p+1) / (q-1),  where q = next prime after p.
   The function factors `n`, applies the formula for each prime power,
   multiplies the contributions, and returns the exact value in type seq_t.
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"   /* for build_primes */

/* --------------------------------------------------------------- */
/* static data: prime table and helper                                    */
static idx_t *prime_table = NULL;
static size_t prime_count = 0;
#define PRIME_LIMIT 20000   /* sufficient for n up to 10000 and next primes */

static void init_primes(void)
{
    if (prime_table != NULL)
        return;                 /* already initialized */

    build_primes(PRIME_LIMIT, &prime_table, &prime_count);
}

/* Find the first prime strictly greater than `p`.
   Assumes prime_table is sorted and contains a prime > p. */
static idx_t next_prime_after(idx_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the index of the first prime > p */
    return (lo < prime_count) ? prime_table[lo] : p + 1;   /* fallback never used for n<=10000 */
}

/* --------------------------------------------------------------- */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety, sequence defined for n>=1 */

    init_primes();

    idx_t m = n;                      /* m will be reduced by prime factors */
    seq_t result = 1;

    /* factor the integer m */
    for (size_t i = 0; i < prime_count && (idx_t)prime_table[i] * prime_table[i] <= m; ++i) {
        idx_t p = prime_table[i];
        if (m % p != 0)
            continue;

        /* exponent e of prime p in m */
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute the contribution for p^e */
        idx_t q = next_prime_after(p);

        /* p^(e-1) */
        seq_t p_pow = 1;
        for (uint64_t k = 1; k < e; ++k)
            p_pow *= (seq_t)p;

        /* q^e */
        seq_t q_pow = 1;
        for (uint64_t k = 0; k < e; ++k)
            q_pow *= (seq_t)q;

        /* numerator: q^e * (p*q-1) - p + 1 */
        seq_t pq_minus1 = (seq_t)p * q - 1;
        seq_t numerator = q_pow * pq_minus1;   /* q^e * (p*q-1) */
        numerator -= (seq_t)p;                 /* subtract p */
        numerator += 1;                        /* add 1 */

        /* complete numerator: numerator * p^(e-1) */
        seq_t term_numer = numerator * p_pow;

        /* denominator: q-1 */
        seq_t denom = (seq_t)(q - 1);

        /* term = term_numer / denom  (exact division) */
        seq_t term = term_numer / denom;

        result *= term;
    }

    /* If a prime factor > sqrt(n) remains */
    if (m > 1) {
        idx_t p = m;
        uint64_t e = 1;

        idx_t q = next_prime_after(p);

        seq_t p_pow = 1;   /* p^(e-1) = p^0 = 1 */
        seq_t q_pow = (seq_t)q;   /* q^1 */

        seq_t pq_minus1 = (seq_t)p * q - 1;
        seq_t numerator = q_pow * pq_minus1;   /* q * (p*q-1) */
        numerator -= (seq_t)p;                 /* -p */
        numerator += 1;                        /* +1 */

        seq_t term_numer = numerator * p_pow; /* just numerator */
        seq_t denom = (seq_t)(q - 1);
        seq_t term = term_numer / denom;

        result *= term;
    }

    return result;
}
