/*
 * A321859  Number of primes congruent to 3, 5, 6 modulo 7 and <= n minus
 *          number of primes congruent to 1, 2, 4 modulo 7 and <= n.
 * https://oeis.org/A321859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (#primes ≤ n with p mod 7 ∈ {3,5,6}) – (#primes ≤ n with
 *            p mod 7 ∈ {1,2,4})
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* a(1) = 0, values below 1 are irrelevant */

    /* Allocate a sieve array: 1 = prime candidate, 0 = composite. */
    unsigned char *is_prime = (unsigned char *)calloc((size_t)(n + 1), sizeof(unsigned char));
    if (!is_prime)
        return 0;                    /* allocation failure – unlikely for n ≤ 10000 */

    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = 1;

    idx_t limit = (idx_t)sqrt((double)n);
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i])
            continue;
        for (idx_t j = i * i; j <= n; j += i)
            is_prime[j] = 0;
    }

    seq_t diff = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p])
            continue;                /* composite */
        if (p == 7)                  /* Legendre symbol (p|7)=0, skip */
            continue;
        idx_t mod7 = p % 7;
        if (mod7 == 3 || mod7 == 5 || mod7 == 6)
            ++diff;                  /* quadratic non‑residue mod 7 */
        else
            --diff;                  /* quadratic residue mod 7 (1,2,4) */
    }

    free(is_prime);
    return diff;
}
