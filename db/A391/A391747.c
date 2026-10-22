/*
 * A391747  a(n) = 1 if -1 <= A195017(n) <= 1, otherwise 0, where A195017(n) =
 *          Sum_{k >= 1} c_k*((-1)^(k-1)), when n = Product_{k >= 1}
 *          (p_k)^(c_k), with p_k the k-th prime.
 * https://oeis.org/A391747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9784, u16 n <= 9784, u32 n <= 9784, u64 n <= 9784,
 *            u128 n <= 9784, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A391747: a(n) = 1 if -1 <= A195017(n) <= 1 else 0.
 * A195017(n) = Σ_k c_k * (-1)^(k-1) where n = Π_k p_k^c_k.
 * The sign pattern is alternating +, -, +, -, … for the primes
 * 2, 3, 5, 7, … respectively.
 *
 * Implementation uses a smallest‑prime‑factor table and a
 * sign table for primes.  The tables are grown lazily as large
 * indices are requested.  The method runs in O(number of
 * prime factors) per call and is well under the required limits
 * for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

/* Static state for lazy table allocation */
static idx_t      *spf         = NULL;   /* smallest prime factor of each number  */
static uint64_t   *prime_list  = NULL;   /* list of primes up to current limit */
static size_t      prime_cnt   = 0;      /* number of primes found               */
static int8_t     *prime_sign  = NULL;   /* +1 or -1 for each prime                */
static idx_t       max_n       = 0;      /* largest n for which tables are ready   */

/* Allocate or grow tables to support indices up to at least n */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n)
        return;

    /* Free old tables if they exist */
    free(spf);
    free(prime_list);
    free(prime_sign);

    /* Build a new smallest‑prime‑factor table up to n */
    idx_t *tmp_spf = NULL;
    build_spf(n, &tmp_spf);
    spf = tmp_spf;                            /* ownership transferred */

    /* Build a new list of primes up to n */
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Allocate sign table for primes */
    size_t size = (size_t)n + 1;              /* indices 0…n inclusive */
    prime_sign = (int8_t *)calloc(size, sizeof(int8_t));
    if (!prime_sign) {
        /* In an unlikely OOM situation, abort – this is a fatal error */
        exit(EXIT_FAILURE);
    }

    /* Fill signs: +1 for odd 1‑based index, -1 for even 1‑based index */
    for (size_t j = 0; j < prime_cnt; ++j) {
        uint64_t p = prime_list[j];
        prime_sign[p] = ((j % 2) == 0) ? 1 : -1;
    }

    max_n = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for n=0, return 0 */

    ensure_capacity(n);

    int sum = 0;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];      /* smallest prime factor */
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        int8_t s = prime_sign[p];
        sum += exp * s;
    }

    return (sum >= -1 && sum <= 1) ? (seq_t)1 : (seq_t)0;
}
