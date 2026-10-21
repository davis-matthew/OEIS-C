/*
 * A252477  Integer part of 1/(sqrt(prime(n+1))-sqrt(prime(n))).
 * https://oeis.org/A252477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/(sqrt(prime(n+1)) - sqrt(prime(n))))
 * Range:     u8 n <= 6542, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, k_le_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/k_le_value.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* out of range – not used by the judge */

    /* Cache the first n+1 primes; rebuild if we need more. */
    static uint64_t *primes = NULL;   /* array of first primes (0‑based) */
    static size_t   prime_cnt = 0;    /* current number of stored primes   */
    static idx_t    current_limit = 0;/* last limit used */

    idx_t needed = n + 1;
    if (prime_cnt < needed) {
        /* Upper bound for the (n+1)-th prime – Dusart estimate */
        idx_t limit;
        if (needed < 6) {
            limit = 15;
        } else {
            double dn = (double)needed;
            limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 10.0);
        }

        if (limit > current_limit) {
            free(primes);
            primes = NULL;
            build_primes_upto(limit, (uint64_t **)&primes, &prime_cnt);
            current_limit = limit;
        }
    }

    /* p_n = primes[n-1], p_{n+1} = primes[n]  (n ≥ 1) */
    uint64_t p1 = primes[n - 1];
    uint64_t p2 = primes[n];

    /* integer square roots of the primes */
    idx_t t1 = _floor_isqrt_u64((idx_t)p1);
    idx_t t2 = _floor_isqrt_u64((idx_t)p2);

    idx_t d   = (idx_t)(p2 - p1);              /* p_{n+1} - p_n */
    idx_t k0  = (t1 + t2) / d;                 /* lower bound for floor */

    idx_t k   = k0;
    idx_t floor_val = k0;

    /* Test successive k until the inequality fails. */
    while (k_le_value(k, p1, p2, d)) {
        floor_val = k;
        /* safety bound – for n up to 10 000 the answer is < 80 */
        if (k > k0 + 10) break;
        ++k;
    }

    return (seq_t)floor_val;
}
