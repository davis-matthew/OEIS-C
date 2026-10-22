/*
 * A338529  a(n) = prime(n+2)*prime(n+3)-prime(n)*prime(n+1).
 * https://oeis.org/A338529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime(n+2)*prime(n+3) - prime(n)*prime(n+1)
 *             (offset 1 – a(1) uses the 1st term listed)
 *
 * The function lazily builds and caches a list of primes up to the
 * required index using the provided build_primes_upto utility.
 * It works for any n >= 1 and keeps the implementation simple and
 * efficient for n up to at least 10 000 (the 10 003rd prime is under
 * 200 000).
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Cached prime list */
static uint64_t *prime_list   = NULL;   /* primes, 0-indexed */
static size_t    prime_count  = 0;       /* number of primes in prime_list */
static idx_t     limit_used   = 0;       /* highest value up to which we sieved */

static void
ensure_primes(idx_t required_index)
{
    /* We need primes up to index 'required_index' (0‑based).  If we already
     * have more primes, do nothing. */
    if (prime_count > required_index) return;

    /* Estimate an upper bound for the (required_index+1)-th prime, using
     * the proven bound p_k ≤ k (ln k + ln ln k) for k ≥ 6. */
    idx_t new_limit;
    if (required_index < 6) {
        new_limit = 15;        /* first primes are below 15 */
    } else {
        double d = (double)required_index;
        new_limit = (idx_t)(d * (log(d) + log(log(d)))) + 3;
    }

    /* Make sure the new limit is strictly larger than any previous limit
     * to avoid repeated allocations when the estimate grows slowly. */
    if (new_limit <= limit_used) {
        new_limit = limit_used * 2;
    }

    /* Re‑build the prime table at the new limit. */
    free(prime_list);
    build_primes_upto(new_limit, &prime_list, &prime_count);
    limit_used = new_limit;

    /* Be defensive: if the estimate was still too small (unlikely), grow
     * until we have enough primes. */
    while (prime_count <= required_index) {
        new_limit = limit_used * 2;
        free(prime_list);
        build_primes_upto(new_limit, &prime_list, &prime_count);
        limit_used = new_limit;
    }
}

seq_t a(idx_t n)
{
    /* a(n) uses primes at indices n-1, n, n+1, n+2 (0‑based).  */
    if (n == 0) {
        /* Undefined for n < 1; but behaviour at offset 0 is irrelevant. */
        return (seq_t)0;
    }

    /* Ensure we have enough primes up to index n+2. */
    ensure_primes(n + 2);

    size_t i = (size_t)n;  /* convert to size_t for array indexing */

    seq_t p_n   = (seq_t)prime_list[i - 1]; /* prime(n)   */
    seq_t p_n1  = (seq_t)prime_list[i];     /* prime(n+1) */
    seq_t p_n2  = (seq_t)prime_list[i + 1]; /* prime(n+2) */
    seq_t p_n3  = (seq_t)prime_list[i + 2]; /* prime(n+3) */

    return p_n2 * p_n3 - p_n * p_n1;
}
