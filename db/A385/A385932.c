/*
 * A385932  Composite numbers m such that the sum of digits of m divides the
 *          sum of digits of prime factors of m (counted with multiplicity).
 * https://oeis.org/A385932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth composite integer m such that sum_of_digits(m)
 *            divides sum_of_digits(prime_factors_with_multiplicity(m))
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/factor_u64.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_cnt = 0;
    static uint64_t next = 4; /* start checking from 4 */

    if (n == 0)
        return 0; /* undefined, but keep for safety */

    if (n <= terms_cnt)
        return terms[n - 1];

    /* Allocate or grow storage for terms if necessary */
    if (!terms) {
        terms_cap = 64;
        terms = (seq_t *)malloc(sizeof(seq_t) * terms_cap);
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    while (terms_cnt < (size_t)n) {
        /* Find next composite number */
        for (; ; ++next) {
            if (next >= 4 && !is_prime_small(next))
                break;
        }

        seq_t sd_m = digit_sum((seq_t)next);
        if (sd_m == 0) { /* should not happen for next >= 4 */
            ++next;
            continue;
        }

        /* Factor next and compute digit sum of prime factors with multiplicity */
        factor_u64(next, primes, exps, &cnt);
        seq_t sd_pf = 0;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t d = digit_sum((seq_t)primes[i]);
            sd_pf += d * (seq_t)exps[i];
        }

        if (sd_m != 0 && sd_pf % sd_m == 0) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * terms_cap);
            }
            terms[terms_cnt++] = (seq_t)next;
        }

        ++next; /* advance to next candidate */
    }

    return terms[n - 1];
}
