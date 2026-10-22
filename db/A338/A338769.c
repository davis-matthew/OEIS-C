/*
 * A338769  Sum of the remainders (p*q mod n) with p,q prime, p + q = n and p
 *          < q.
 * https://oeis.org/A338769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p<q, p+q=n, p,q prime} (p*q mod n)
 * Range:     u8 n <= 35, u16 n <= 1559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a prime sieve incrementally and sums the
   remainders for all distinct prime pairs whose sum is the argument n.
   It works for any seq_t and idx_t types supplied by the build. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static bool *prime_flags = NULL;   /* sieve: true = prime */
static idx_t prime_flags_len = 0;  /* length of prime_flags array */

/* Expand the sieve so that it contains primes up to at least N (inclusive). */
static void ensure_prime_flags(idx_t N)
{
    if (N < prime_flags_len)
        return;              /* already sufficient */

    size_t sz = (size_t)N + 1;
    bool *flags = (bool*)malloc(sz * sizeof(bool));
    if (!flags)
        exit(EXIT_FAILURE); /* allocation failure */

    for (size_t i = 0; i < sz; ++i)
        flags[i] = true;

    if (N >= 0) flags[0] = false;
    if (N >= 1) flags[1] = false;

    /* Classic Sieve of Eratosthenes */
    for (idx_t i = 2; i <= N / i; ++i) {
        if (!flags[i])
            continue;
        for (idx_t j = i * i; j <= N; j += i)
            flags[j] = false;
    }

    free(prime_flags);
    prime_flags   = flags;
    prime_flags_len = sz;
}

seq_t a(idx_t n)
{
    /* Ensure primes up to n are known */
    ensure_prime_flags(n);

    /* For all primes p < q with p+q = n */
    idx_t limit = (n - 1) / 2;   /* ensures p < q */
    seq_t sum = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!prime_flags[p] || !prime_flags[n - p])
            continue;            /* both must be prime, and p < q */

        idx_t q = n - p;
        seq_t prod = (seq_t)p * (seq_t)q;   /* product fits in seq_t */
        seq_t rem  = prod % (seq_t)n;       /* remainder modulo n */
        sum += rem;
    }

    return sum;
}
