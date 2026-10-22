/*
 * A353445  Let f be the completely multiplicative function from the positive
 *          integers to the cube roots of unity defined by f(prime(m)) =
 *          w^(2^(m-1)), where w is the cube root with positive imaginary
 *          part. a(n) is twice the real part of f(n).
 * https://oeis.org/A353445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 if the weighted sum of the exponents of the prime
 *            factorization of n is congruent to 0 modulo 3; otherwise a(n) =
 *            -1. The weight of a prime p = prime(m) is (-1)^(m-1), i.e. +1
 *            for odd prime indices and -1 for even prime indices. This
 *            implementation obtains the parity of the prime index b...
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Static storage for the list of primes used to determine parity of
   prime indices.  The list is stored in ascending order.  It is
   expanded lazily as larger prime factors are encountered. */
static uint64_t *prime_list    = NULL;   /* array of primes */
static size_t    prime_count   = 0;      /* number of primes in the list */
static idx_t     prime_limit   = 0;      /* maximum prime value stored */

/* Return true iff p is the 1‑based odd index prime (i.e. its index is
   1,3,5, …).  p is guaranteed to be prime. */
static bool
odd_prime_index(const uint64_t p)
{
    /* Ensure the prime list covers all primes up to p.  If p exceeds the
       current limit, rebuild the list up to p.  Rebuilding appends a
       fresh array; we free the old one to avoid memory waste. */
    if (p > prime_limit)
    {
        uint64_t *new_list   = NULL;
        size_t    new_count  = 0;

        build_primes_upto(p, &new_list, &new_count);

        free(prime_list);
        prime_list  = new_list;
        prime_count = new_count;
        prime_limit = p;
    }

    /* Binary search the index of p in the prime list.  Since p is a
       factor of n, it will appear in the list. */
    size_t lo = 0, hi = prime_count;
    while (lo < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }

    /* lo is the 0‑based index of p.  1‑based index is lo+1, which is
       odd exactly when lo is even. */
    return (lo % 2) == 0;
}

seq_t
a(idx_t n)
{
    /* Trivial case: a(1) = 2. */
    if (n == 1)
        return (seq_t)2;

    /* Factor n into primes.  The library function factor_u64 provides
       prime factors and their exponents.  The array can hold up to 64
       distinct primes – more than enough for all 64‑bit unsigned
       integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* Compute the weighted sum modulo 3.  For each prime factor p with
       exponent e, add e * (-1)^(index(p)-1) modulo 3. */
    uint64_t sum_mod3 = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i] % 3;          /* use only e mod 3 */
        bool     odd = odd_prime_index(p);   /* true if index is odd */

        /* If the prime index is odd, the weight is +1; otherwise it
           is -1 (which is 2 modulo 3).  We add the contribution
           modulo 3. */
        if (odd)
            sum_mod3 = (sum_mod3 + e) % 3;
        else
            sum_mod3 = (sum_mod3 + (e == 0 ? 0 : 3 - e)) % 3;
    }

    return (seq_t)((sum_mod3 == 0) ? 2 : -1);
}
