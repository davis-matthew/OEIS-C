/*
 * A273462  Rounded variance of the first n primes, for n > 1.
 * https://oeis.org/A273462
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( Sum_{i=1..n} (prime(i) - Sum_{j=1..n}
 *            prime(j)/n)^2/(n - 1) )
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 385, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_arr = NULL;   /* array of the first primes (uint32_t) */
static size_t prime_cnt   = 0;       /* currently stored number of primes */

/* Ensure that prime_arr contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt)
        return;                       /* we already have enough */

    /* free previous allocation (if any) */
    if (prime_arr)
        free(prime_arr);

    sieve_first_n_primes(n, &prime_arr, &prime_cnt);
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* a(0) and a(1) are undefined, return 0 */

    ensure_primes(n);

    seq_t sum    = 0;
    seq_t sum_sq = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t p = (seq_t)prime_arr[i];
        sum    += p;
        sum_sq += p * p;
    }

    seq_t numerator = (seq_t)n * sum_sq - sum * sum;
    seq_t denom     = (seq_t)n * ((seq_t)n - 1);

    /* banker's rounding: round to nearest integer, ties go to even */
    seq_t q = numerator / denom;      /* integer part */
    seq_t r = numerator % denom;      /* remainder */
    seq_t double_r = r * ((seq_t)2);
    seq_t result;

    if (double_r < denom)
        result = q;
    else if (double_r > denom)
        result = q + 1;
    else        /* exact half */
    {
        if ((q & 1) == 0)
            result = q;
        else
            result = q + 1;
    }

    return result;
}
