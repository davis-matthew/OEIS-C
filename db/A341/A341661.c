/*
 * A341661  Primes p such that p^4 - 1 has fewer than 160 divisors.
 * https://oeis.org/A341661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes_upto, divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A341661
   Primes p such that p^4 - 1 has fewer than 160 divisors.  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

/* a(n) returns the n-th prime p for which p^4 - 1 has < 160 divisors.
   The sequence is finite: only the first 18 primes satisfy the rule. */
seq_t a(idx_t n)
{
    /* cache the computed sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!seq) {
        /* build all primes up to the last one that qualifies (101) */
        const idx_t limit = 101;
        uint64_t *primes = NULL;
        size_t cnt = 0;

        /* primes will be stored in a dynamically allocated array */
        build_primes_upto(limit, &primes, &cnt);

        /* allocate maximum possible size (cnt) and fill qualifying primes */
        seq = (seq_t *)malloc(cnt * sizeof(seq_t));
        seq_len = 0;

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            /* compute p^4 - 1 using 64-bit arithmetic */
            uint64_t p2   = p * p;
            uint64_t p4   = p2 * p2;
            uint64_t m    = p4 - 1;
            /* count divisors */
            if (divisor_count_u64(m) < 160) {
                seq[seq_len++] = (seq_t)p;
            }
        }

        free(primes);
    }

    if (n == 0)          /* n must be >= 1, but guard anyway */
        return 0;
    if (n <= seq_len)
        return seq[n - 1];
    return 0;            /* no more terms */
}
