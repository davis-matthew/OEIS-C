/*
 * A329571  a(n) = Product_{prime p} p^floor(log_p P) with P = A329570(n) the
 *          least prime with log_p P >= valuation(n,p) for all primes p.
 * https://oeis.org/A329571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{prime p <= P} p^⌊log_p P⌋, where P is the least
 *            prime ≥ M and M = max_{q|n} q^{v_q(n)}. (Equivalently, P =
 *            A329570(n) and a(n) = A003418(P))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64, isPrimeGeneric,
 *            next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cache of all primes up to the largest P seen so far */
    static uint64_t *cached_primes = NULL;
    static size_t   cached_cnt     = 0;
    static uint64_t cached_limit   = 0; /* largest P for which we have primes */

    if (n == 0) return 0;         /* offset 1 starts at n=1; value for 0 is irrelevant */

    /* -------------------------------------------------------------
       1. Determine M = max_{q|n} q^{v_q(n)}.
       ------------------------------------------------------------- */
    uint64_t primes_f[128];
    uint64_t exps_f[128];
    size_t   cnt_f = 0;
    if (n > 1) {
        factor_u64((uint64_t)n, primes_f, exps_f, &cnt_f);
    }

    uint64_t M = 1;
    for (size_t i = 0; i < cnt_f; ++i) {
        uint64_t p  = primes_f[i];
        uint64_t e  = exps_f[i];
        uint64_t pow = 1;
        for (uint64_t k = 0; k < e; ++k) pow *= p;
        if (pow > M) M = pow;
    }

    /* -------------------------------------------------------------
       2. Find the least prime P >= M.
       ------------------------------------------------------------- */
    uint64_t P = M;
    if (!isPrimeGeneric(P)) {
        /* search for the next prime >= M */
        uint64_t cand = P + 1;
        while (!isPrimeGeneric(cand)) ++cand;
        P = cand;
    }

    /* -------------------------------------------------------------
       3. Ensure we have all primes up to P cached.
       ------------------------------------------------------------- */
    if (P > cached_limit) {
        free(cached_primes);
        cached_primes = NULL;
        cached_cnt = 0;
        cached_limit = 0;

        size_t new_cnt = 0;
        uint64_t *new_primes = NULL;
        build_primes_upto(P, &new_primes, &new_cnt);

        cached_primes = new_primes;
        cached_cnt    = new_cnt;
        cached_limit  = P;
    }

    /* -------------------------------------------------------------
       4. Compute product_{prime p <= P} p^⌊log_p P⌋.
       ------------------------------------------------------------- */
    seq_t result = (seq_t)1;
    seq_t seqP   = (seq_t)P;

    for (size_t idx = 0; idx < cached_cnt; ++idx) {
        uint64_t q   = cached_primes[idx];
        if (q > P) break;

        seq_t seq_q = (seq_t)q;
        seq_t exp   = (seq_t)0;
        seq_t pow   = (seq_t)1;
        while (pow * seq_q <= seqP) {
            pow *= seq_q;
            ++exp;
        }

        result *= big_pow(seq_q, exp);
    }

    return result;
}
