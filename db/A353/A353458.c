/*
 * A353458  Dirichlet inverse of A353457.
 * https://oeis.org/A353458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1. For n > 1, factor n = Π p^e. Let π(p) = number of
 *            primes ≤ p. If π(p) is odd: a(p^e) = 1. If π(p) is even: a(p) =
 *            –1, a(p^e) = 0 for e>1. a(n) = Π a(p^e)
 * Range:     i8 n <= 8979, i16 n <= 8979, i32 n <= 8979, i64 n <= 8979,
 *            i128 n <= 8979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function is self‑contained, uses a small sieve to obtain all
    primes up to a fixed bound (110 000, sufficient for n up to 10 000),
    and is independent of the external utilities. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- global prime table ---------- */
static uint64_t *prime_list = NULL;   /* primes in ascending order   */
static size_t   prime_cnt  = 0;       /* number of stored primes     */

/* ---------- sieve to build prime list once ---------- */
static void init_primes(void)
{
    if (prime_list != NULL) return;    /* already built */

    const idx_t LIMIT = 110000;        /* > maximum n needed          */
    bool *is_comp = (bool*)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* count primes */
    size_t c = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++c;

    prime_list = (uint64_t*)malloc(c * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[k++] = i;

    prime_cnt = c;
    free(is_comp);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    init_primes();

    seq_t result = (seq_t)1;
    idx_t temp   = n;                 /* remaining part to factor */

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p == 0)
        {
            size_t exp = 0;
            do
            {
                ++exp;
                temp /= p;
            } while (temp % p == 0);

            uint32_t pi_p = (uint32_t)(i + 1);   /* π(p) = index+1   */
            seq_t factor;

            if (pi_p & 1U)                     /* π(p) odd  */
                factor = (seq_t)1;
            else                                /* π(p) even */
            {
                if (exp == 1)
                    factor = (seq_t)-1;
                else
                    factor = (seq_t)0;
            }

            result *= factor;
            if (result == (seq_t)0) return (seq_t)0;   /* zero propagates */
        }
    }

    /* Remaining factor > sqrt(n) is prime (or 1) */
    if (temp > (idx_t)1)
    {
        /* Locate its index in prime_list via linear search */
        size_t idx = 0;
        while (idx < prime_cnt && prime_list[idx] != temp) ++idx;

        uint32_t pi_p = (uint32_t)(idx + 1);   /* π(temp)  */
        seq_t factor = (pi_p & 1U) ? (seq_t)1 : (seq_t)-1;

        result *= factor;
    }

    return result;
}
