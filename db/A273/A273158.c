/*
 * A273158  Number of concatenations nm consisting of n followed by a positive
 *          integer m (not a multiple of 10) that are divisible by m.
 * https://oeis.org/A273158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 42, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   digit_len, factor_u64, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = #{ m > 0 , 10 ∤ m |  ( n || m ) % m == 0 } 
   i.e. m divides n * 10^digits(m) and does not end in 0.  */

/*  Note: uses helper utilities from the OEIS framework.
    All arithmetic is performed with type seq_t.             */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* factorisation routine for the part of n coprime to 10      */
#include "../utils/factor_u64.h"

/* basic integer operations on seq_t                         */
#include "../utils/seq_mul.h"

/* power of 10, digit length for seq_t, prime factor count   */
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not required but safe */

    /* 1.  Separate powers of 2 and 5 from n */
    idx_t e2 = 0, e5 = 0;
    idx_t tmp = n;

    while ((tmp & 1ULL) == 0) {      /* factor 2 */
        tmp >>= 1;
        e2++;
    }
    while (tmp % 5ULL == 0) {        /* factor 5 */
        tmp /= 5ULL;
        e5++;
    }
    idx_t n_prime = tmp;              /* part coprime to 10 */

    /* 2.  Enumerate all divisors of n_prime (t values) */
    size_t div_cnt = 1;
    idx_t *divs = (idx_t *)malloc(sizeof(idx_t));
    divs[0] = 1;

    if (n_prime > 1) {
        uint64_t primes[64], exps[64];
        size_t pf_cnt = 0;
        factor_u64((uint64_t)n_prime, primes, exps, &pf_cnt);

        for (size_t pidx = 0; pidx < pf_cnt; ++pidx) {
            uint64_t p = primes[pidx];
            int    e = (int)exps[pidx];
            size_t old_cnt = div_cnt;
            size_t new_cnt = old_cnt * (size_t)(e + 1);
            idx_t *new_divs = (idx_t *)malloc(sizeof(idx_t) * new_cnt);

            for (size_t j = 0; j < old_cnt; ++j) {
                idx_t val = divs[j];
                for (int k = 0; k <= e; ++k) {
                    new_divs[j * (size_t)(e + 1) + k] = val;
                    if (k < e) val *= p;
                }
            }
            free(divs);
            divs   = new_divs;
            div_cnt = new_cnt;
        }
    }

    /* 3.  Count suitable m values */
    seq_t count = 0;

    for (size_t ti = 0; ti < div_cnt; ++ti) {
        idx_t t = divs[ti];

        /* --- family A : m = t * 2^i (i >= 0) --- */
        seq_t pow2 = 1;                     /* 2^i */
        for (idx_t i = 0; ; ++i) {
            seq_t m = seq_mul((seq_t)t, pow2);
            int d = digit_len(m);           /* digits(m) */

            /* divisibility condition on exponents of 2 */
            if (i > (idx_t)(e2 + d)) break;

            seq_t prod = seq_mul((seq_t)n, pow10((idx_t)d));
            if ((prod % m) == 0) count++;

            pow2 = seq_mul(pow2, (seq_t)2);
        }

        /* --- family B : m = t * 5^j (j >= 1) --- */
        seq_t pow5 = 1;                     /* 5^j, start from j=1 */
        for (idx_t j = 1; ; ++j) {
            pow5 = seq_mul(pow5, (seq_t)5);
            seq_t m = seq_mul((seq_t)t, pow5);
            int d = digit_len(m);

            /* divisibility condition on exponents of 5 */
            if (j > (idx_t)(e5 + d)) break;

            seq_t prod = seq_mul((seq_t)n, pow10((idx_t)d));
            if ((prod % m) == 0) count++;
        }
    }

    free(divs);
    return count;
}
