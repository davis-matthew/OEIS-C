/*
 * A325973  Arithmetic mean of {sum of unitary divisors} and {sum of
 *          squarefree divisors}: a(n) = (1/2) * (A034448(n) + A048250(n)).
 * https://oeis.org/A325973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A034448(n)+A048250(n)) / 2 (Arithmetic mean of the
 *            unitary sum and the square‑free sum) Implemented by factoring
 *            each n, then unitary_sum = product(1 + p^e) over all distinct
 *            prime powers p^e || n sqfree_sum = product(1 + p) over all
 *            distinct primes p dividing n The result is (unitary_sum...
 * Range:     u8 n <= 137, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9399 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple trial‑division factorisation for 64‑bit numbers.
   On return *cnt is the number of distinct prime factors found; the
   corresponding primes are stored in fp[i] and their exponents in fe[i].
   The caller may provide arrays of any length ≥ 64.              */
static void factor_u64_custom(idx_t n, uint64_t fp[], int fe[], idx_t *cnt)
{
    *cnt = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            fp[(*cnt)] = p;
            fe[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1)
    {
        fp[(*cnt)] = tmp;
        fe[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* Main sequence function.  idx_t is uint64_t; seq_t is an unsigned type. */
seq_t a(idx_t n)
{
    /* a(1) is 1 by definition (unitary sum = sum of squarefree = 1). */
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    int      exps[64];
    idx_t    num_factors;

    factor_u64_custom(n, primes, exps, &num_factors);

    seq_t unitary_sum = 1;
    seq_t sqfree_sum  = 1;

    for (idx_t i = 0; i < num_factors; ++i)
    {
        seq_t p   = (seq_t)primes[i];
        int   e   = exps[i];

        /* Compute p^e in seq_t */
        seq_t p_pow = 1;
        for (int t = 0; t < e; ++t)
            p_pow *= p;

        unitary_sum *= (seq_t)(1) + p_pow;
        sqfree_sum  *= (seq_t)(1) + p;
    }

    /* Arithmetic mean of the two sums.  Division by 2 is exact
       because both sums have the same parity. */
    return (unitary_sum + sqfree_sum) / (seq_t)2;
}
