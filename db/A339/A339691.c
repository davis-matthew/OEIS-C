/*
 * A339691  a(n) is the least prime p > prime(n+1) such that p == -prime(n+1)
 *          (mod prime(n)).
 * https://oeis.org/A339691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least prime p > prime(n+1) such that p ≡ -prime(n+1) (mod
 *            prime(n))
 * Range:     u8 n <= 9, u16 n <= 285, u32 n <= 900, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implements the definition directly.  It builds a cache
 * of the first primes needed and then, for each n, finds the smallest
 * prime in the arithmetic progression congruent to -prime(n+1) (mod prime(n))
 * that is greater than prime(n+1).  All arithmetic is performed in seq_t
 * so the routine works for all supported seq_t sizes.  The routine is
 * memoised and works correctly even if calls arrive in arbitrary order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build environment */
#include "../utils/is_prime_big.h"
#include "../utils/_prime_limit_for_idx.h"   /* function _prime_limit_for_idx(idx_t) */
#include "../utils/build_primes_upto.h"      /* function build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt) */

/* ---------- static data to cache a list of primes ---------- */

static uint64_t *prime_list   = NULL;  /* prime_list[0] = prime(1) */
static size_t   prime_count   = 0;     /* total primes stored */

/* ------------------------------------------------------------------ */
/* raise the prime cache to include at least the first "need" primes   */
/* "need" is 1‑based (need ≥ 1).                                       */
/* ------------------------------------------------------------------ */
static void ensure_prime_cache(idx_t need)
{
    if ((size_t)need <= prime_count) /* already have enough */
        return;

    /* Determine a limit that is guaranteed to contain at least "need" primes.
     * We repeatedly double the limit if the initial estimate fails. */
    idx_t          limit   = _prime_limit_for_idx(need);
    uint64_t      *newlist = NULL;
    size_t        newcnt  = 0;

    for (;;)
    {
        /* First try the estimated limit. */
        build_primes_upto(limit, &newlist, &newcnt);
        if (newcnt >= (size_t)need)
            break;                         /* success */

        /* Not enough primes – enlarge the limit and retry.  Free the earlier
         * buffer to avoid memory leak. */
        free(newlist);
        newlist = NULL;
        newcnt  = 0;
        if (limit < 1000000)
            limit = limit * 2 + 100;        /* simple growth strategy */
        else
            limit = limit + 100000;         /* grow a bit slower for very large limits */
    }

    /* Replace the old list with the new one. */
    free(prime_list);
    prime_list = newlist;
    prime_count = newcnt;
}

/* ------------------------------------------------------------------ */
/* public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)                 /* behaviour below the offset is irrelevant */
        return 0;

    /* Ensure we have both prime(n) and prime(n+1). */
    ensure_prime_cache(n + 1);

    uint64_t q = prime_list[n - 1]; /* prime(n)   */
    uint64_t r = prime_list[n];    /* prime(n+1) */

    /* r % q gives the residue of r modulo q. */
    uint64_t r_mod = r % q;
    /* The required residue for p is (q - r_mod) % q. */
    uint64_t target = (q - r_mod) % q;

    /* Compute the offset from r to the first number ≡ target (mod q). */
    uint64_t offset = (target + q - r_mod) % q;
    if (offset == 0)
        offset = q;                 /* need a number strictly greater than r */

    seq_t candidate = (seq_t)r + (seq_t)offset;

    /* Find the first prime in the arithmetic progression. */
    while (!is_prime_big(candidate))
        candidate += (seq_t)q;

    return candidate;
}
