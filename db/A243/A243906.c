/*
 * A243906  (Number of semiprimes <= n) - (number of primes <= n).
 * https://oeis.org/A243906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A072000(n) - A000720(n) (number of semiprimes ≤ n minus
 *            number of primes ≤ n)
 * Range:     i8 n <= 964, i16 n <= 5195, i32 n <= 5195, i64 n <= 5195,
 *            i128 n <= 5195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) for n >= 1.  The function builds a linear sieve up to n,
 * counts primes and numbers with exactly two prime factors (omega = 2),
 * and returns the difference.  Memory is freed before returning. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* A(1)=0, equal to the first term */

    /* Allocate the smallest‑prime‑factor array and zero‑initialize it. */
    uint32_t *spf = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
    if (!spf)
        return 0;   /* allocation failure – return 0 as a safe default */

    /* Allocate space for the list of primes (worst case length = n). */
    uint32_t *primes = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!primes) {
        free(spf);
        return 0;
    }

    /* Allocate an omega array to count prime factors with multiplicity. */
    uint32_t *omega = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!omega) {
        free(spf);
        free(primes);
        return 0;
    }

    size_t prime_cnt = 0;

    /* Linear Sieve: fill spf array and list of primes. */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || i > n / p)
                break;
            spf[i * p] = p;
        }
    }

    /* Count primes and semiprimes. */
    size_t primeCount = 0;
    size_t semiprimeCount = 0;
    omega[1] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
        if (spf[i] == (uint32_t)i)
            ++primeCount;
        if (omega[i] == 2)
            ++semiprimeCount;
    }

    free(spf);
    free(primes);
    free(omega);

    return (seq_t)semiprimeCount - (seq_t)primeCount;
}
