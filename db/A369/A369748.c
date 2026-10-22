/*
 * A369748  a(n) = Sum_{p|n, p prime} prime(n/p).
 * https://oeis.org/A369748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} prime(n/p)
 * Range:     u8 n <= 77, u16 n <= 6509, u32 n <= 9459, u64 n <= 9459,
 *            u128 n <= 9459, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a sieve to compute smallest prime factors
   and a list of prime values up to an absolute limit. The prime at
   index k (k ≥ 1) is stored in prime_arr[k‑1].  The function a(n)
   works for any n ≥ 1 and caches its helper tables between calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Prototypes of helper functions from the utils directory.          */
/* These headers provide the declarations needed for the compiler.   */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(uint64_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/build_spf.h"           /* build_spf(uint64_t limit, uint64_t **spf_out) */

/* Cached tables.  They are initialized on the first call to a()   */
static uint64_t *prime_arr = NULL;   /* prime values array */
static size_t    prime_cnt = 0;      /* number of primes in prime_arr */

static uint64_t *spf_arr   = NULL;   /* smallest prime factor array */
static uint64_t   spf_limit = 0;     /* maximum index covered by spf_arr */

/* Ensure that prime_arr contains enough primes and that spf_arr
   covers all indices up to the requested limit.                 */
static void init_tables(idx_t requested_limit)
{
    /* Build the prime list if it hasn't been done yet.              */
    if (!prime_arr) {
        /* A value of 200000 is more than enough to provide the first
           5000 primes (needed for n up to ~10000).                 */
        build_primes_upto(200000, &prime_arr, &prime_cnt);
    }

    /* Build or extend the SPF table as necessary.                   */
    if (!spf_arr || spf_limit < requested_limit) {
        uint64_t *new_spf = NULL;
        build_spf(requested_limit, &new_spf);
        free(spf_arr);  /* free old table if this is an extension     */
        spf_arr   = new_spf;
        spf_limit = requested_limit;
    }
}

/* Compute a(n).  n is guaranteed to be ≥ 1 for ordinary usage.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* outside the defined domain */

    /* Ensure helper tables are large enough for this n.          */
    init_tables(n);

    seq_t sum = 0;
    idx_t temp = n;

    while (temp > 1) {
        /* Smallest prime factor of temp */
        idx_t p = spf_arr[temp];

        /* Add the prime numbered n/p.  n/p is always ≥ 1.   */
        idx_t q = n / p;
        /* q-1 is a valid index into prime_arr because
           prime_arr was built with a value limit > 200000. */
        sum += (seq_t)(prime_arr[q - 1]);

        /* Skip all occurrences of p in temp.  */
        while (temp % p == 0)
            temp /= p;
    }

    return sum;
}
