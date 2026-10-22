/*
 * A348156  S_3-primes: let S_3 = {1,4,7,...,3i+1,...}; then an S_3-prime is
 *          in S_3 but is not divisible by any elements of S_3 except for
 *          itself and 1.
 * https://oeis.org/A348156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* S3-primes: generate via trial division on previously found S3-primes. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t *s3p = NULL;         /* stored S3-primes           */
static size_t s3cnt = 0;          /* number of primes stored   */
static size_t s3cap = 0;          /* allocated capacity        */
static idx_t  next_k = 1;         /* next k for candidate 3*k+1 */

static void grow_s3(void) {
    if (s3cnt >= s3cap) {
        size_t newcap = (s3cap == 0) ? 32 : s3cap * 2;
        seq_t *tmp = (seq_t *)realloc(s3p, newcap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        s3p   = tmp;
        s3cap = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safe fallback for offset 1 */

    /* Generate S3-primes until we have at least n of them. */
    while (s3cnt < (size_t)n) {
        seq_t cand = (seq_t)3 * (seq_t)next_k + 1;
        next_k++;

        /* Test divisibility by all known S3-primes ≤ √cand. */
        seq_t lim = floor_sqrt(cand);
        int isprime = 1;
        for (size_t i = 0; i < s3cnt && s3p[i] <= lim; ++i) {
            if (cand % s3p[i] == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime) {
            grow_s3();
            s3p[s3cnt++] = cand;
        }
    }
    return s3p[n - 1];
}
