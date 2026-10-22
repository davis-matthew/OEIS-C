/*
 * A390328  Numbers k such that the number of prime divisors of binomial(2k,
 *          k), including multiplicities, is odd.
 * https://oeis.org/A390328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A390328.
    a(n) = the nth integer k (k ≥ 1) such that the total number
    of prime divisors of binomial(2k, k) (with multiplicity)
    is odd.
    The parity of Ω(C(2k,k)) is computed multiplicatively:
        e_p = Σ_{i≥1} ( ⌊2k/p^i⌋ – 2·⌊k/p^i⌋ )
    The sum of all e_p modulo 2 is the desired parity.  */

/*  The implementation keeps a growing cache of primes up to 2k
    and of the sequence terms found so far; it works for any
    build that supplies idx_t and seq_t.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"   /* Provides:  void sieve_primes(idx_t limit, idx_t **out, size_t *cnt); */

static idx_t *prime_cache      = NULL;   /* primes up to prime_limit */
static size_t prime_cnt        = 0;
static idx_t   prime_limit     = 0;       /* maximum k*2 for which primes are available */

static seq_t *seq_cache  = NULL;         /* sequence terms */
static size_t seq_len    = 0;
static size_t seq_cap    = 0;
static idx_t  next_k     = 1;             /* next k to test */

static void ensure_primes(idx_t needed_limit)
{
    if (needed_limit <= prime_limit)
        return;
    if (prime_cache != NULL)
        free(prime_cache);
    sieve_primes(needed_limit, &prime_cache, &prime_cnt);
    prime_limit = needed_limit;
}

static bool parity_of_binom(idx_t k)
{
    idx_t twok = k << 1;
    ensure_primes(twok);

    bool parity = false;    /* false = even, true = odd */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_cache[i];
        if (p > twok)
            break;

        idx_t pow = p;
        while (pow <= twok)
        {
            idx_t term = (twok / pow) - ((k / pow) << 1);
            if (term & 1U)
                parity = !parity;

            if (pow > twok / p)    /* would overflow on next multiplication */
                break;
            pow *= p;
        }
    }
    return parity;
}

seq_t a(idx_t n)
{
    /* Grow term cache if necessary */
    if (seq_cache == NULL)
    {
        seq_cap = 128;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < (size_t)n)
    {
        if (parity_of_binom(next_k))
        {
            if (seq_len == seq_cap)
            {
                seq_cap <<= 1; /* double capacity */
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
