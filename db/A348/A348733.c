/*
 * A348733  a(n) = gcd(A003959(n), A034448(n)), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and A034448 (usigma) is
 *          multiplicative with a(p^e) = (p^e)+1.
 * https://oeis.org/A348733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003959(n), A034448(n))
 * Range:     u8 n <= 47, u16 n <= 6335, u32 n <= 9884, u64 n <= 9884,
 *            u128 n <= 9884, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A003959(n)=∏(p+1)^e over n=p^e, and A034448(n)=∏(p^e+1) over n=p^e */
/* Implementation factorises n, computes both multiplicative values, and takes gcd */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

static uint64_t *prime_list = NULL;     /* array of primes, up to prime_limit */
static size_t prime_count  = 0;         /* number of primes in prime_list      */
static idx_t    prime_limit  = 0;       /* current maximal n for which primes */
static void     init_primes(idx_t limit) {
    if (limit <= prime_limit)         /* already prepared for this range */
        return;

    /* rebuild prime list up to limit */
    bool *sieve = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!sieve) return;               /* out of memory – leave empty */

    /* initialise sieve */
    for (idx_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                sieve[k] = false;
        }
    }

    /* count primes and allocate storage */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *pg = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pg) {
        free(sieve);
        return;                     /* out of memory – leave old list */
    }

    /* store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) pg[pos++] = (uint64_t)i;

    /* replace old list */
    free(prime_list);
    prime_list   = pg;
    prime_count  = cnt;
    prime_limit  = limit;

    free(sieve);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but safe */
    if (n == 1) return 1;            /* a(1)=1 */

    /* ensure prime list goes up to n (to factor n efficiently) */
    init_primes(n);

    seq_t f1 = 1;                    /* A003959(n) */
    seq_t f2 = 1;                    /* A034448(n) */

    uint64_t tmp = (uint64_t)n;
    for (size_t i = 0; i < prime_count && prime_list[i] <= tmp / prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p != 0) continue;

        /* compute exponent e of p in n */
        unsigned int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* contribution to A003959: (p+1)^e */
        seq_t term1 = seq_pow((seq_t)(p + 1), e);
        f1 *= term1;

        /* contribution to A034448: (p^e + 1) */
        seq_t pow_p = seq_pow((seq_t)p, e);
        seq_t term2 = pow_p + (seq_t)1;
        f2 *= term2;
    }

    /* remaining factor > sqrt(original n), if any (must be prime) */
    if (tmp > 1) {
        seq_t term1 = (seq_t)(tmp + 1);
        seq_t term2 = (seq_t)(tmp + 1);   /* because e==1 */
        f1 *= term1;
        f2 *= term2;
    }

    return gcd(f1, f2);
}
