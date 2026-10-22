/*
 * A318573  Numerator of the reciprocal sum of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A318573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of the sum of the reciprocals of the
 *            indices of the primes that factor n. That is, if n = ∏ p_j^{e_j}
 *            and index(p_j) is the position of prime p_j (prime(1)=2,
 *            prime(2)=3, …), then a(n) = numerator of ∑ (e_j / index(p_j)) in
 *            lowest terms
 * Range:     u8 n <= 2488, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t , seq_t ); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t , uint64_t *, uint64_t *, size_t *); */
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* static cache of primes up to the largest n seen so far */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_last = 0;   /* largest prime in the cache */

    /* enlarge prime cache if needed */
    if (primes == NULL || prime_last < n) {
        free(primes);                /* free old cache (no-op if NULL) */
        build_primes_upto(n, &primes, &prime_cnt);
        prime_last = primes[prime_cnt - 1];
    }

    /* factor n */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pf, ex, &pcnt);

    /* sum the fractions e / index(p) */
    seq_t num = (seq_t)0;   /* numerator of running sum   */
    seq_t den = (seq_t)1;   /* denominator of running sum */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pf[i];
        uint64_t e = ex[i];

        /* find the index of prime p (1‑based) via binary search */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] >= p) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        /* primes[lo] == p because p is a prime factor of n */
        size_t y_index = lo + 1;   /* make it 1‑based */

        seq_t y = (seq_t)y_index;
        seq_t exp = (seq_t)e;

        /* add exp/y to the running sum */
        seq_t new_num = num * y + den * exp;
        seq_t new_den = den * y;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
