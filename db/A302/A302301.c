/*
 * A302301  Number of ways to write n as a sum of two distinct semiprimes.
 * https://oeis.org/A302301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = ∑_{i=1..⌊(n-1)/2⌋} [Ω(i)=2] * [Ω(n-i)=2]
 * Range:     u8 n <= 3779, u16 n <= 9918, u32 n <= 9918, u64 n <= 9918,
 *            u128 n <= 9918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Ω is the big‐Omega function, i.e. the number of prime factors counted with multiplicity) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static state */
static idx_t max_n = 0;          /* maximum n for which we have precomputed data */
static idx_t *spf = NULL;        /* smallest prime factor array */
static uint8_t *omega_arr = NULL;/* Ω(n) values */
static bool *is_semiprime = NULL;/* true iff the number is a semiprime (Ω = 2) */

/* Recompute sieve data up to 'limit' (inclusive) */
static void recompute_semiprimes(idx_t limit)
{
    /* Free old data if present */
    if (spf)     free(spf);
    if (omega_arr)  free(omega_arr);
    if (is_semiprime) free(is_semiprime);

    /* Allocate new arrays */
    spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    omega_arr = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    is_semiprime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!spf || !omega_arr || !is_semiprime) abort();

    /* Initialise */
    memset(spf, 0, (limit + 1) * sizeof(idx_t));
    memset(is_semiprime, 0, (limit + 1) * sizeof(bool));

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i) {        /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* Compute Ω(n) */
    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        omega_arr[i] = omega_arr[i / spf[i]] + 1;
    }

    /* Mark semiprimes */
    for (idx_t i = 2; i <= limit; ++i) {
        if (omega_arr[i] == 2) is_semiprime[i] = 1;
    }

    max_n = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure sieve data covers n */
    if (n > max_n) recompute_semiprimes(n);

    seq_t count = 0;

    /* For n == 0 no loop is executed; formula yields 0 automatically */
    if (n == 0) return 0;

    /* Limit: floor((n-1)/2) */
    idx_t limit = (n - 1) / 2;

    for (idx_t i = 1; i <= limit; ++i) {
        if (is_semiprime[i] && is_semiprime[n - i]) {
            ++count;
        }
    }

    return count;
}
