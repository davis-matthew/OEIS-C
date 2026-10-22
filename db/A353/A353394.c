/*
 * A353394  Product of prime shadows of prime indices of n (with
 *          multiplicity).
 * https://oeis.org/A353394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} A181819(index_of_prime(p))^e (A181819(m) =
 *            product of primes indexed by the exponents in the factorization
 *            of m)
 * Range:     u8 n <= 6560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/*--- Prime table ---------------------------------------------------------*/
static uint64_t *prime_list = NULL; /* primes[0] = prime(1) = 2 */
static size_t   prime_cnt   = 0;     /* number of primes in prime_list   */
static idx_t    prime_limit = 0;     /* maximum prime value currently stored */

/* Build or extend the prime table up to at least 'lim'. */
static void ensure_primes_upto(idx_t lim) {
    if (lim <= prime_limit) return;
    free(prime_list);
    build_primes_upto(lim, &prime_list, &prime_cnt);
    prime_limit = lim;
}

/* Return the 1‑based index of a prime p (assumes p is prime). */
static idx_t index_of_prime(uint64_t p) {
    ensure_primes_upto(p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo + 1);   /* 1‑based */
}

/* Return the prime with 1‑based position 'pos'. */
static seq_t prime_at(idx_t pos) {
    if (pos == 0) return (seq_t)0;
    /* Ensure enough primes for the exponent values. */
    if (pos > (idx_t)prime_cnt) {
        /* Default limit covers the needed range for exponent values. */
        ensure_primes_upto(1000000);
    }
    return (seq_t)prime_list[pos - 1];
}

/*--- A181819 (prime shadow) ---------------------------------------------*/
static seq_t prime_shadow(idx_t m) {
    if (m <= 1) return (seq_t)1;
    uint64_t f[32], e[32];
    size_t cnt = 0;
    factor_u64((uint64_t)m, f, e, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t exp = (idx_t)e[i];
        seq_t p_exp = prime_at(exp);
        res *= p_exp;
    }
    return res;
}

/*--- Main sequence -------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined; safe for completeness */

    /* Build primes up to n to guarantee index_of_prime works for all
       prime factors of n.  This keeps the table size moderate for the
       typical n≤10000 used in the timing tests. */
    ensure_primes_upto(n);

    uint64_t pf[32], exps[32];
    size_t num = 0;
    factor_u64((uint64_t)n, pf, exps, &num);

    seq_t result = 1;
    for (size_t i = 0; i < num; ++i) {
        uint64_t p = pf[i];
        uint64_t e = exps[i];
        idx_t idx = index_of_prime(p);
        seq_t shadow = prime_shadow(idx);
        for (uint64_t k = 0; k < e; ++k) {
            result *= shadow;
        }
    }
    return result;
}
