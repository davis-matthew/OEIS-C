/*
 * A354233  Least number with n runs in ordered prime signature.
 * https://oeis.org/A354233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the product of the first n primes with exponents alternating
 * 1 and 2, starting with exponent 1 when n is odd and with exponent 2 when
 * n is even.  Thus for n ≥ 1 the nth term is
 *
 *   a(n) = ∏_{i=1..n} p_i^{e_i} ,
 *
 * where p_i is the i‑th prime and
 *
 *   e_i = { 1 if (n is odd  AND i is odd) or (n is even AND i is even),
 *          2 otherwise } .
 *
 * The sequence begins:
 *   1, 2, 12, 90, 2100, 48510, 3303300, 139369230, 18138420300, …
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;      /* array of first primes found */
static size_t primes_cnt = 0;        /* number of primes currently stored */
static size_t primes_cap = 0;        /* allocated capacity for primes */

static void ensure_primes(idx_t n)
{
    /* Generate primes until we have at least n primes. */
    while (primes_cnt < n)
    {
        uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        while (true)
        {
            bool is_prime = true;
            for (size_t j = 0; j < primes_cnt; ++j)
            {
                uint64_t p = primes[j];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            ++candidate;
        }
        /* Grow storage if needed */
        if (primes_cnt >= primes_cap)
        {
            primes_cap = primes_cap ? primes_cap * 2 : 256;
            uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) abort();   /* allocation failed */
            primes = tmp;
        }
        primes[primes_cnt++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    ensure_primes(n);

    seq_t result = (seq_t)1;

    /* Determine starting exponent based on parity of n */
    bool n_is_even = (n % 2 == 0);

    for (idx_t i = 0; i < n; ++i)
    {
        uint8_t exp;
        /* i is 0‑indexed; position i+1 corresponds to prime index i+1 */
        if (n_is_even)
            exp = (i % 2 == 0) ? 2 : 1;   /* odd positions get 2, even get 1 */
        else
            exp = (i % 2 == 0) ? 1 : 2;   /* odd positions get 1, even get 2 */

        seq_t prime_val = (seq_t)primes[(size_t)i];
        seq_t factor   = (exp == 1) ? prime_val : (prime_val * prime_val);
        result = result * factor;
    }

    return result;
}
