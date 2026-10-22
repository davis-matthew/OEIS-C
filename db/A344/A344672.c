/*
 * A344672  a(n) = Sum_{primes p <=n} phi(floor(n/p)).
 * https://oeis.org/A344672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p <= n, p prime} phi(floor(n/p))
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function seq_t calc_phi(seq_t m); */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n < 2)            /* no primes <= 1 */
        return (seq_t)0;

    /* ---------- Sieve of Eratosthenes up to n ---------- */
    size_t sz = (size_t)(n + 1);
    uint8_t *composite = (uint8_t *)malloc(sz);
    if (!composite)
        return (seq_t)0;   /* allocation failed; return 0 as fallback */

    memset(composite, 0, sz);

    /* dynamic array for primes */
    size_t primes_cap = 0;
    size_t primes_cnt = 0;
    uint64_t *primes = NULL;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (!composite[i])               /* i is prime */
        {
            /* store the prime */
            if (primes_cnt == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 4;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp)                /* allocation failed */
                {
                    free(primes);
                    free(composite);
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint64_t)i;

            /* mark multiples of i */
            if (i <= n / i)              /* avoid i*i overflow */
                for (idx_t j = i * i; j <= n; j += i)
                    composite[j] = 1;
        }
    }

    /* ---------- Compute the sum ---------- */
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k < primes_cnt; ++k)
    {
        idx_t p = (idx_t)primes[k];
        idx_t m = n / p;               /* floor(n/p) */
        seq_t phi_val = calc_phi((seq_t)m);
        sum += phi_val;
    }

    free(primes);
    free(composite);
    return sum;
}
