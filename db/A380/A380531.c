/*
 * A380531  a(n) is the multiplicative order of -4 modulo prime(n); a(1) = 0
 *          for completion.
 * https://oeis.org/A380531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the multiplicative order of -4 modulo the n-th prime;
 *            a(1)=0
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_minus_four, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/mod_minus_four.h"

static uint64_t *prime_tbl = NULL;      /* array of primes */
static size_t prime_cnt  = 0;           /* number of primes stored */

/* Make sure we have at least n primes precomputed */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    /* Estimate an upper bound for the n-th prime */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn   = (double)n;
        double lim  = dn * (log(dn) + log(log(dn))) + 3.0; /* proven upper bound for n ≥ 6 */
        limit = (idx_t)lim;
    }

    build_primes_upto(limit, &prime_tbl, &prime_cnt);
}

/* Multiplicative order of -4 modulo p (p odd prime) */
static seq_t order_of_minus_four(uint64_t p)
{
    if (p == 2) return 0; /* not used, but keep for safety */

    seq_t base = mod_minus_four(p);
    seq_t order = (seq_t)p - 1;   /* maximal possible order */
    seq_t temp  = order;

    /* Factorise order and reduce it using the standard algorithm */
    for (idx_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= temp; ++i) {
        uint64_t q = prime_tbl[i];
        if (temp % q != 0) continue;
        while (temp % q == 0) {
            seq_t cand = temp / q;
            seq_t r = mod_pow(base, (uint64_t)cand, (seq_t)p);
            if (r == 1) {
                temp = cand;
            } else {
                break;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t cand = temp / temp;   /* = 1 */
        seq_t r = mod_pow(base, (uint64_t)cand, (seq_t)p);
        if (r == 1) temp = cand;
    }

    return temp;
}

/* Main sequence function requested by the problem specification. */
seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* completion value for the first index */

    ensure_primes(n);
    if (n > prime_cnt) return 0;   /* fallback, should not happen for n ≤ 10000 */

    uint64_t p = prime_tbl[n - 1];  /* nth prime */
    return order_of_minus_four(p);
}
