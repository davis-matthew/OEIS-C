/*
 * A334172  Bitwise XNOR of prime(n) and prime(n + 1).
 * https://oeis.org/A334172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Bitwise XNOR of prime(n) and prime(n + 1)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8623 (2026-11-01)
 * Helpers:   bit_length, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Computes the nth term of OEIS A334172 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1)                /* safety for out of bound indices */
        return (seq_t)0;

    /* We need prime(n) and prime(n+1) ==> at least n+1 primes */
    idx_t target   = n + 1;
    idx_t limit    = _prime_limit_for_idx(target);
    uint64_t *primes = NULL;
    size_t cnt = 0;

    /* Build the array of primes up to a suitable limit. */
    build_primes_upto(limit, &primes, &cnt);

    /* Retrieve the two consecutive primes. */
    seq_t p1 = (seq_t)primes[n - 1];   /* prime(n)   */
    seq_t p2 = (seq_t)primes[n];       /* prime(n+1) */

    /* Bitwise XNOR: ~(p1 ^ p2) masked to the width of the larger prime. */
    seq_t xor_val = p1 ^ p2;
    idx_t bitlen  = (idx_t)bit_length(p1 | p2);  /* max bitwidth of p1 and p2 */

    seq_t mask;
    if (bitlen == (idx_t)(sizeof(seq_t) * 8))
        mask = ~ (seq_t)0;                      /* all ones when bitlen == word size */
    else
        mask = ((seq_t)1 << bitlen) - 1;        /* (1<<bitlen)-1 */

    seq_t result = (~xor_val) & mask;

    free(primes);
    return result;
}
