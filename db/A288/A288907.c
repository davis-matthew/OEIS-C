/*
 * A288907  Primes p whose distance from the next prime and from the previous
 *          prime is less than log(p).
 * https://oeis.org/A288907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that * p – prev(p) < log(p) and
 *            next(p) – p < log(p)
 * Range:     u8 n <= 9, u16 n <= 2278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function lazily builds a list of such primes up to a
 * fixed sieve limit and returns the n-th element. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static bool initialized = false;
static seq_t *good = NULL;     /* array of qualifying primes */
static size_t good_len = 0;    /* number of elements in good */

/* --------------------------------------------------------------------
 * a(idx_t n) – returns the n-th element of sequence A288907
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Sieve limit – large enough to contain at least 10 000 terms
         * of the sequence. Adjust if a higher number of terms is
         * required. */
        const idx_t LIMIT = 10000000ULL;   /* 10 000 000 */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;

        /* Generate all primes up to LIMIT.  build_primes_upto returns
         * an array of uint64_t primes and the count. */
        build_primes_upto((idx_t)LIMIT, &primes, &prime_cnt);

        /* Reserve space for at most prime_cnt-2 qualifying primes. */
        seq_t *tmp = (seq_t *)malloc((prime_cnt > 2 ? prime_cnt - 2 : 0) * sizeof(seq_t));
        size_t tmp_cnt = 0;

        /* Scan each inner prime and test the two gap conditions. */
        for (size_t i = 1; i + 1 < prime_cnt; ++i) {
            uint64_t p      = primes[i];
            uint64_t left   = primes[i - 1];
            uint64_t right  = primes[i + 1];
            uint64_t gap_l  = p - left;
            uint64_t gap_r  = right - p;

            /* floor(log(p)) : use double precision; the truncation is safe
             * because p << DBL_MAX and gaps are integers. */
            double logp = log((double)p);
            uint64_t floorlog = (uint64_t)logp;

            if (gap_l <= floorlog && gap_r <= floorlog) {
                tmp[tmp_cnt++] = (seq_t)p;
            }
        }

        /* Shrink the array to its true size. */
        good = (seq_t *)malloc(tmp_cnt * sizeof(seq_t));
        if (good != NULL) {
            for (size_t i = 0; i < tmp_cnt; ++i) {
                good[i] = tmp[i];
            }
        }
        good_len = tmp_cnt;

        /* Clean up temporary storage. */
        free(tmp);
        free(primes);

        initialized = true;
    }

    /* Return the n-th element if it exists; otherwise return 0. */
    if (n == 0 || (size_t)n > good_len)
        return (seq_t)0;
    return good[(size_t)n - 1];
}
