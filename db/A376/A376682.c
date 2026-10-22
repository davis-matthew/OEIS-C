/*
 * A376682  Array read by antidiagonals downward where A(n,k) is the n-th term
 *          of the k-th differences of the noncomposite numbers (A008578).
 * https://oeis.org/A376682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A376682 is read from the array A(i,j) which contains the * k‑th
 *            differences of the non‑composite numbers A008578. For * 0‑based
 *            index n the antidiagonal coordinates are * k such that k(k+1)/2
 *            <= n < (k+1)(k+2)/2, * j = n - k(k+1)/2, * i = k - j. * A(i,j) =
 *            Σ_{t=0..j} (-1)^(j-t) * C(j,t) * A008578...
 * Range:     i8 n <= 43, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binomial, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/binomial.h"

/* Pre‑allocate enough non‑composite numbers (1 + primes). */
static seq_t noncomposite[600];
static size_t nc_len = 0;

static void init_noncomposite(void)
{
    if (nc_len != 0)
        return;
    /* 1 is the first non‑composite by definition. */
    noncomposite[0] = (seq_t)1;
    nc_len = 1;

    /* Generate enough primes for the required size. */
    const idx_t prime_limit = 20000;  /* 20000th prime ≈ 224737, more than enough. */
    uint64_t *primes = NULL;
    size_t pcount = 0;
    build_primes_upto(prime_limit, &primes, &pcount);

    /* Store primes until the array is full. */
    for (size_t i = 0; i < pcount && nc_len < 600; ++i)
        noncomposite[nc_len++] = (seq_t)primes[i];

    free(primes);
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_noncomposite();
        initialized = true;
    }

    /* Find k such that k(k+1)/2 <= n < (k+1)(k+2)/2. */
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n)
        ++k;

    idx_t j = n - k * (k + 1) / 2;  /* j <= k */
    idx_t i = k - j;                /* i >= 0 */

    seq_t sum = 0;
    for (idx_t t = 0; t <= j; ++t) {
        seq_t bin = binomial(j, t);           /* binomial(j,t) */
        if (((j - t) & 1U) != 0)              /* (-1)^(j-t) */
            bin = -bin;
        sum += bin * noncomposite[i + t];
    }
    return sum;
}
