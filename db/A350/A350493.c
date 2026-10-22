/*
 * A350493  a(n) = floor(sqrt(prime(n)))^2 mod n.
 * https://oeis.org/A350493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(prime(n)))^2 mod n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS definition using built‑in primitive types and
   the helpers provided in the utils directory. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t n)   */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **out, size_t *cnt_out); */

/* ------------------------------------------------------------------ */
/* Prime caching utilities                                            */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL; /* Cached primes (1‑based access)   */
static size_t   prime_cnt  = 0;     /* Number of primes cached          */
static size_t   sieve_limit = 0;    /* Current upper bound of sieve     */

/* Ensure that at least n primes are cached.  The sieve limit is
   doubled until the required number of primes is available. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;                       /* already sufficient */

    /* Pick an initial limit.  For very small n the 15‑prime limit is fine;
       for larger n we start with a rough upper bound 20·n. */
    if (sieve_limit == 0)
        sieve_limit = (n < 6) ? 15 : (size_t)(n * 20);

    while (prime_cnt < (size_t)n) {
        /* Double the sieve region. */
        sieve_limit *= 2;

        uint64_t *new_list = NULL;
        size_t new_cnt = 0;
        build_primes_upto(sieve_limit, &new_list, &new_cnt);

        /* Replace the old list. */
        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
    }
}

/* ------------------------------------------------------------------ */
/*   OEIS sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, but harmless */

    ensure_primes(n);

    /* Fetch the n‑th prime (1‑based). */
    uint64_t p = prime_list[n - 1];

    /* Compute floor(sqrt(p)) using the helper that works on 64‑bit
       integers. */
    idx_t k = _floor_isqrt_u64(p);

    /* Square the integer square root. */
    uint64_t sq = (uint64_t)k * (uint64_t)k;

    /* Modulo n.  (n <= 10000 for the tested range, so 64‑bit holds.) */
    uint64_t result = sq % (uint64_t)n;

    /* Convert to the build‑dependent seq_t type. */
    return (seq_t)result;
}
