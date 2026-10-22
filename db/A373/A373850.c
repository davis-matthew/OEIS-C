/*
 * A373850  a(n) = A276085(n) mod bigomega(n), where bigomega (A001222) and
 *          A276085 are fully additive with a(p) = 1 and a(p) = p#/p
 *          respectively.
 * https://oeis.org/A373850
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276085(n) mod bigomega(n), where A276085 is additive
 *            with a(p) = p#/p (p# = product of all primes ≤ p)
 * Range:     u8 n <= 9179, u16 n <= 9179, u32 n <= 9179, u64 n <= 9179,
 *            u128 n <= 9179, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;         /* list of primes (up to the highest limit requested) */
static size_t    prime_cnt  = 0;         /* number of primes */
static idx_t     prime_limit = 0;       /* maximum prime generated so far */

/* Ensure that all primes up to |limit| are available.  Called lazily. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;            /* already have enough */

    uint64_t *new_primes;
    size_t new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);

    free(primes);                                 /* release the old array */
    primes     = new_primes;
    prime_cnt  = new_cnt;
    prime_limit = limit;
}

/* Compute the sequence value for a single n.  n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    ensure_primes_upto(n);          /* primes up to n are now available */

    idx_t  temp   = n;
    idx_t  omega  = 0;               /* bigomega(n) */
    idx_t  fac_primes[64];           /* primes dividing n (distinct) */
    idx_t  fac_exps [64];           /* corresponding exponents */
    size_t numf   = 0;               /* number of distinct prime factors */

    /* Trial division using the pre‑computed primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if ((idx_t)p * p > temp) break;
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            fac_primes[numf] = p;
            fac_exps [numf] = e;
            ++numf;
            omega += e;
        }
    }
    if (temp > 1) {                       /* remaining prime factor > sqrt(n) */
        fac_primes[numf] = temp;
        fac_exps [numf] = 1;
        ++numf;
        ++omega;
    }

    /* Modulus is bigomega(n).  All arithmetic is done modulo omega. */
    seq_t mod = (seq_t)omega;
    seq_t sum = 0 % mod;                  /* behaves correctly even if mod==1 */

    for (size_t i = 0; i < numf; ++i) {
        idx_t p = fac_primes[i];
        idx_t e = fac_exps [i];

        /* Compute product of all primes < p modulo omega. */
        seq_t prod = 1 % mod;
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t q = primes[j];
            if (q < p) {
                prod = (prod * (seq_t)q) % mod;
            } else break;
        }

        seq_t add = ((seq_t)e % mod) * prod % mod;
        sum = (sum + add) % mod;
    }

    return sum;
}
