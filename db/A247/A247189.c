/*
 * A247189  a(n) is the least integer k such that prime(k) < (prime(n)+
 *          prime(n+1) + ... + prime(n+k))/k <= prime(k+1).
 * https://oeis.org/A247189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer k such that * prime(k) <
 *            (prime(n)+prime(n+1)+...+prime(n+k))/k <= prime(k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches incrementally for k, using a prime sieve. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static bool primes_initialized = false;
static uint64_t *primes_vals = NULL;
static size_t   primes_count = 0;

/* Initialise the list of primes once.  A limit of 200 000 covers
 * all indices needed for n <= 10 000 with a generous margin on k. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    const idx_t LIMIT = 200000;          /* about 17 000 primes */
    uint64_t *tmp = NULL;
    size_t   cnt  = 0;

    build_primes_upto(LIMIT, &tmp, &cnt);

    primes_vals  = tmp;
    primes_count = cnt;
    primes_initialized = true;
}

/* Return the n-th term of sequence A247189 (1‑based).  If n <= 0 the
 * function returns 0; out‑of‑range indices are also handled gracefully. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* outside the defined offset */
        return 0;

    init_primes();

    if ((size_t)n > primes_count)       /* safety guard */
        return 0;

    /* sum of primes from p_n onward */
    idx_t sum = primes_vals[n - 1];     /* p_n */

    for (idx_t k = 1; ; ++k) {
        /* indices of the needed primes */
        idx_t idx_pk  = n + k - 1;       /* prime(n + k)   */
        idx_t idx_pk1 = n + k;           /* prime(n + k + 1) */

        /* ensure we have enough pre‑computed primes */
        if ((size_t)idx_pk1 >= primes_count)
            return 0;                   /* should not happen with LIMIT */

        /* add prime(n + k) to the cumulative sum */
        sum += primes_vals[idx_pk];

        /* retrieve the relevant primes */
        idx_t pk  = primes_vals[idx_pk];
        idx_t pk1 = primes_vals[idx_pk1];

        /* test the inequality */
        if (sum > pk * k && sum <= pk1 * k)
            return (seq_t)k;
    }
}
