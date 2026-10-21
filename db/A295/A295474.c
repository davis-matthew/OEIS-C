/*
 * A295474  a(n) = (20*n)!*(7*n)!*(2*n)!/((14*n)!*(10*n)!*(4*n)!*n!).
 * https://oeis.org/A295474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n)!*(7*n)!*(2*n)!/((14*n)!*(10*n)!*(4*n)!*n!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

/* static state for prime lookup */
static uint64_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes */
static idx_t    primes_limit = 0; /* maximum number for which primes are known */

/* Ensure that all primes up to `limit` are generated */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    uint64_t *new_primes = NULL;
    size_t   new_cnt     = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);
    if (primes != NULL)
        free(primes);
    primes      = new_primes;
    primes_cnt  = new_cnt;
    primes_limit = limit;
}

/* Legendre's formula: exponent of prime `p` in `m!` */
static inline idx_t exp_in_factorial(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0U)            /* a(0) = 1 */
        return (seq_t)1;

    /* prepare primes up to 20*n */
    ensure_primes_upto((idx_t)20 * n);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        idx_t exp =
            exp_in_factorial((idx_t)20 * n, p) +
            exp_in_factorial((idx_t)7 * n, p) +
            exp_in_factorial((idx_t)2 * n, p) -
            exp_in_factorial((idx_t)14 * n, p) -
            exp_in_factorial((idx_t)10 * n, p) -
            exp_in_factorial((idx_t)4 * n, p) -
            exp_in_factorial(n, p);
        if (exp > 0) {
            seq_t pw = seq_pow((seq_t)p, exp);
            result *= pw;
        }
    }
    return result;
}
