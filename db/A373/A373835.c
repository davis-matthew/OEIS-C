/*
 * A373835  a(n) = gcd(bigomega(n), A276085(n)), where bigomega (A001222), and
 *          A276085 are fully additive with a(p) = 1 and a(p) = p#/p
 *          respectively.
 * https://oeis.org/A373835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8712, u16 n <= 8712, u32 n <= 8712, u64 n <= 8712,
 *            u128 n <= 8712, big any n
 * Helpers:   bigomega, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A373835:  a(n) = gcd( bigomega(n), A276085(n) )
   where bigomega(n) is the total number of prime factors of n (with multiplicity)
   and A276085 is the additive function defined by a(p) = p# / p = product of
   all primes less than p.  The implementation computes A276085(n) modulo
   bigomega(n) so that no large intermediate products are involved. */
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;      /* primes up to current limit */
static size_t prime_cnt = 0;
static idx_t  prime_limit = 0;           /* current limit of prime_list */

seq_t a(idx_t n)
{
    /* n is at least 1 for all valid calls.  bigomega(1) returns 0. */
    seq_t omega_seq = bigomega(n);
    /* For n ≥ 1, bigomega(n) ≤ 64 for all practical n; cast safely. */
    unsigned int omega = (unsigned int)omega_seq;
    if (omega == 0)                /* n == 1 → a(1) = 0 */
        return (seq_t)0;

    /* Ensure we have a list of all primes up to n. */
    if (n > prime_limit) {
        build_primes_upto(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    /* Factor n and obtain primes and exponents. */
    uint64_t fac_pr[64]; /* enough for n up to very large values */
    uint64_t fac_ex[64];
    size_t fac_cnt;
    factor_u64((uint64_t)n, fac_pr, fac_ex, &fac_cnt);

    /* Compute A276085(n) modulo omega. */
    unsigned int sum_mod = 0;

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        unsigned int exp = (unsigned int)fac_ex[i];
        /* product of primes < p modulo omega */
        unsigned int prod_mod = 1 % omega;
        for (size_t j = 0; j < prime_cnt && prime_list[j] < p; ++j)
            prod_mod = (unsigned int)((unsigned int)prod_mod *
                                      (prime_list[j] % omega)) % omega;
        sum_mod += (unsigned int)((unsigned long long)exp * prod_mod % omega);
        sum_mod %= omega;               /* keep it reduced */
    }

    /* gcd(omega, sum_mod).  Both are small enough for 32‑bit arithmetic. */
    unsigned int a_val = omega;
    unsigned int b_val = sum_mod;
    while (b_val != 0) {
        unsigned int r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    return (seq_t)a_val;
}
