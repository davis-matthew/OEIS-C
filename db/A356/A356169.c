/*
 * A356169  a(n) = n - gcd(n, A003961(A356164(n))), where A356164(n) is the
 *          smallest positive k such that n divides k*A003961(k), and A003961
 *          is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A356169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n, A003961(A356164(n))); where A356164(n) is the
 *            smallest k>0 such that n | k*A003961(k). A003961 is
 *            multiplicative with a(p)=nextprime(p)
 * Range:     u8 n <= 279, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   factor_u64, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the function described in the OEIS entry. */

#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/gcd.h"         /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t) */

/* Compute A003961(k) exactly (as seq_t), using prime factorisation of k. */
static seq_t A003961_exact(uint64_t k)
{
    if (k == 1)
        return (seq_t)1;

    /* Prime factorization of k */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;

    factor_u64(k, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t np = next_prime(p);
        for (uint64_t j = 0; j < e; ++j)
            prod *= (seq_t)np;
    }
    return prod;
}

/* Return the smallest k such that n | k*A003961(k). */
static uint64_t A356164(uint64_t n)
{
    /* k cannot exceed n, because k = n trivially satisfies the condition. */
    for (uint64_t k = 1; k <= n; ++k) {
        seq_t A = A003961_exact(k);          /* exact value */
        seq_t prod_mod = (seq_t)((k % n) * (A % (seq_t)n) % (seq_t)n);
        if (prod_mod == 0)
            return k;
    }
    return n;   /* Fallback (should not happen) */
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* convert index to native type */

    uint64_t k = A356164(nn);           /* compute A356164(n) */
    seq_t A = A003961_exact(k);         /* exact A003961(k) */

    seq_t g = gcd((seq_t)nn, A);        /* gcd(n, A003961(k)) */

    return (seq_t)nn - g;
}
