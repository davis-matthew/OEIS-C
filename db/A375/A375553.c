/*
 * A375553  a(n) is the smallest prime q such that the concatenation (p + q)"q
 *          is a prime number, where p = prime(n).
 * https://oeis.org/A375553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest prime q such that concat((prime(n)+q),q) is
 *            prime
 * Range:     u8 n <= 4, u16 n <= 291, u32 n <= 291, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   build_primes_upto, concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

/* Cached list of primes: used for both p = prime(n) and for searching q. */
static uint64_t *prime_list   = NULL;     /* array of primes (ascending)       */
static size_t   prime_count   = 0;        /* number of primes stored           */
static idx_t    prime_limit   = 0;        /* last sieve limit used (in digits) */

/* Ensure that prime_list contains at least n primes (1‑based).  Rebuild
   the list up to a limit that yields at least n primes. */
static void ensure_primes_at_least(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    idx_t limit = prime_limit ? prime_limit : 100;
    while (true) {
        uint64_t *tmp   = NULL;
        size_t   cnt   = 0;
        build_primes_upto(limit, &tmp, &cnt);

        /* Free old list if any.  The new list will replace it. */
        free(prime_list);
        prime_list = tmp;
        prime_count = cnt;
        prime_limit = limit;

        if (cnt >= (size_t)n) return;   /* we now have enough primes */

        /* Need more primes: increase the sieve limit.  Double the limit
           to keep the growth factor < 2, which is fast enough for the
           ranges we care about. */
        limit <<= 1;
    }
}

/* Main entry for the OEIS sequence.  The function follows the
   requirements: external linkage, no static `main`, no I/O, uses only
   allowed helpers, and computes `a(n)` for any n ≥ 1. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; handle n==0 robustly. */
    if (n == (idx_t)0) return 0;

    /* Ensure we have the first n primes so that prime(n) is available. */
    ensure_primes_at_least(n);
    uint64_t p = prime_list[n - 1];   /* prime(n), 1‑based index */

    /* Search for the smallest prime q such that concat(p+q, q) is prime. */
    idx_t q_index = 0;                /* index into prime_list for q       */

    for (;;) {
        /* If we have exhausted the current prime list, grow it. */
        if (q_index >= prime_count) {
            idx_t new_limit = prime_limit ? prime_limit << 1 : 100;
            uint64_t *tmp   = NULL;
            size_t   cnt    = 0;
            build_primes_upto(new_limit, &tmp, &cnt);

            free(prime_list);
            prime_list = tmp;
            prime_count = cnt;
            prime_limit = new_limit;
        }

        uint64_t q = prime_list[q_index++];
        /* Compute candidate = concat(p+q, q).  The helper expects
           idx_t (i.e., uint64_t) inputs; the sum p+q is guaranteed
           to fit in this range for the problem limits. */
        seq_t candidate = concat_parts((idx_t)(p + q), (idx_t)q);

        if (is_prime_big(candidate))
            return (seq_t)q;               /* found the smallest q */
    }
}
