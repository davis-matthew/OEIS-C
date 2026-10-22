/*
 * A377621  a(n) is the number of iterations of x -> 6*x - 1 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) counts iterations of x → 6x‑1 until the number of primes seen equals
 *  the number of composites seen, starting with the n‑th prime.  The returned
 *  value is the number of such iterations.  The implementation uses only
 *  integer arithmetic and a primality test for arbitrary precision numbers.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &out, &cnt)   */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n)             */

static uint64_t *prime_arr = NULL;   /* array of primes, 1‑based index      */
static size_t prime_cnt   = 0;        /* number of primes stored             */

/* Ensure the array contains at least n primes.  The bound 20*n+100 is
 * more than enough for n up to at least 10⁶ (the millionth prime is
 * 15,485,863).  If the array is not large enough we rebuild it. */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* We re‑allocate from scratch each time we need more primes for simplicity. */
    idx_t limit = 20 * n + 100;
    free(prime_arr);
    prime_arr = NULL;

    size_t cnt = 0;
    build_primes_upto(limit, &prime_arr, &cnt);
    prime_cnt = cnt;
}

/* Main function as required by the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n = 0; return 0 as a harmless default */
        return 0;

    /* Make sure the prime array contains the n‑th prime. */
    ensure_primes(n);

    seq_t x = (seq_t)prime_arr[n - 1];    /* nth prime, 1‑based index */

    seq_t primes_seen    = 1; /* the starting prime is counted */
    seq_t composites_seen = 0;
    seq_t steps = 0;

    while (true) {
        /* Next term in the sequence.  Use seq_t arithmetic to avoid
         * overflow for arbitrary‑precision builds. */
        x = 6 * x - 1;

        /* Check if the new term is prime. */
        if (is_prime_big(x))
            ++primes_seen;
        else
            ++composites_seen;

        ++steps;                  /* we have performed one more iteration */

        /* Stop when the numbers of primes and composites are equal. */
        if (primes_seen == composites_seen)
            return steps;
    }
}
