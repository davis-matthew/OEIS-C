/*
 * A294731  Smallest average of a twin prime pair divisible by the n-th prime,
 *          i.e. A090530(n), divided by 6*prime(n).
 * https://oeis.org/A294731
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A090530(n) / (6 * prime(n)) for n ≥ 3, where A090530(n)
 *            is the smallest average of a twin prime pair divisible by the
 *            n-th prime. The implementation searches successive multiples of
 *            6·prime(n) until the number minus 1 and plus 1 are both prime
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 65, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/build_primes.h"  /* build_primes(idx_t limit, idx_t **out, size_t *cnt) */

static idx_t *prime_array = NULL;   /* array of primes, 0‑based */
static size_t prime_count = 0;      /* number of primes stored */

/* Ensure that prime_array contains at least num_primes primes.
   We pre‑allocate a fixed upper bound sufficient for n ≤ 10000. */
static void ensure_primes(idx_t needed)
{
    if (prime_array != NULL && needed <= prime_count)
        return;                     /* already enough */

    /* All required primes will be below 200 000 for n ≤ 10 000. */
    size_t limit = 200000;
    build_primes((idx_t)limit, &prime_array, &prime_count);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)                         /* undefined for offset range */
        return 0;

    /* Make sure we have the n-th prime. */
    ensure_primes(n);
    if (n > prime_count)               /* safety check */
        return 0;

    /* nth prime (1‑based) */
    seq_t p = (seq_t)prime_array[n - 1];

    /* Step size: 6·p.  For t = 1, average = step. */
    seq_t step = (seq_t)6 * p;
    seq_t avg  = step;
    seq_t t    = (seq_t)1;

    for (;; ++t, avg += step) {
        if (is_prime_big(avg - 1) && is_prime_big(avg + 1))
            return t;                  /* a(n) equals this t */
    }
}
