/*
 * A295466  a(n) = (15*n)!*(6*n)!*n!/((12*n)!*(5*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   fact_prime_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (15*n)!*(6*n)!*n!/((12*n)!*(5*n)!*(3*n)!*(2*n)!)
 *              = binomial(15*n,3*n)*binomial(6*n,n)/binomial(2*n,n)
 *              = ∏_{p prime ≤ 15n} p^(E(p)),
 *                where E(p) = v_p((15n)!)+(v_p((6n)!)+v_p(n!))
 *                              -v_p((12n)!)-v_p((5n)!)-v_p((3n)!)-v_p((2n)!).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/fact_prime_exponent.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t limit = 15 * n;
    /* Sieve of Eratosthenes to list primes up to limit */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return 0;   /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;
    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) { free(is_prime); return 0; }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;
    free(is_prime);

    seq_t result = 1;

    for (size_t k = 0; k < prime_cnt; ++k) {
        idx_t p = primes[k];

        /* compute exponent E(p) */
        idx_t e = fact_prime_exponent(15 * n, p);
        e += fact_prime_exponent(6 * n, p);
        e += fact_prime_exponent(n, p);
        e -= fact_prime_exponent(12 * n, p);
        e -= fact_prime_exponent(5 * n, p);
        e -= fact_prime_exponent(3 * n, p);
        e -= fact_prime_exponent(2 * n, p);

        /* multiply result by p^e, one p at a time to avoid overflow */
        for (idx_t i = 0; i < e; ++i)
            result *= (seq_t)p;
    }

    free(primes);
    return result;
}
