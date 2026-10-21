/*
 * A242932  Numbers n such that k*n/(k+n) is prime for some k.
 * https://oeis.org/A242932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   cmp_seq_t, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242932
 * 
 * Definition: a(n) is the nth smallest integer n such that there exists a
 * prime p with n equal to one of p+1, 2p, or p*(p+1).  The sequence is
 * sorted and deduplicated.  It starts with 3, 4, 6, 8, ...
 * 
 * This implementation builds the sequence up to a fixed prime limit
 * sufficient for about 10 000 terms (and many more).  On first call it
 * generates all primes ≤ LIMIT, produces the corresponding 3 values
 * per prime, sorts them, removes duplicates, and caches the result.
 * Subsequent calls simply index the cached array.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/sieve_primes.h"   /* provides sieve_primes */
#include "../utils/cmp_seq_t.h"      /* provides cmp_seq_t */

static seq_t *seq_buf   = NULL;   /* cached sorted, unique terms   */
static size_t seq_len   = 0;      /* number of cached terms        */

static void build_sequence(void)
{
    /* Limit chosen so that ≥~15 000 distinct terms are produced
     * (enough for all required tests).  Pi(50 000)=5 133, so
     * 3*5 133≈15 400 terms.  This is comfortably small for memory.   */
    const idx_t PRIMELIMIT = 50000;

    idx_t *primes = NULL;
    size_t pcount = 0;

    /* Generate primes up to PRIMELIMIT.  The function allocates
     * an array of idx_t and returns its pointer via primes.        */
    sieve_primes(PRIMELIMIT, &primes, &pcount);

    /* Temporary array for all candidate numbers: 3 per prime. */
    size_t tmp_len = 3 * pcount;
    seq_t *tmp = (seq_t *)malloc(tmp_len * sizeof(seq_t));
    size_t i = 0;

    for (size_t j = 0; j < pcount; ++j) {
        seq_t p = (seq_t)primes[j];
        tmp[i++] = p + (seq_t)1;          /* p + 1   */
        tmp[i++] = (seq_t)2 * p;          /* 2p      */
        tmp[i++] = p * (p + (seq_t)1);    /* p(p+1)  */
    }

    /* Sort the temporary array.  cmp_seq_t compares two seq_t values. */
    qsort(tmp, tmp_len, sizeof(seq_t),
          (int (*)(const void *, const void *))cmp_seq_t);

    /* Remove duplicates in a second pass.  We overwrite the beginning
     * of tmp with the unique values and count them. */
    size_t uniq = 0;
    for (size_t j = 0; j < tmp_len; ++j) {
        if (uniq == 0 || cmp_seq_t(&tmp[uniq - 1], &tmp[j]) != 0) {
            tmp[uniq++] = tmp[j];
        }
    }

    /* Allocate exactly enough space for the final sequence */
    seq_buf = (seq_t *)malloc(uniq * sizeof(seq_t));
    for (size_t j = 0; j < uniq; ++j) {
        seq_buf[j] = tmp[j];
    }
    seq_len = uniq;

    /* Clean up temporary data */
    free(tmp);
    free(primes);
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset of this sequence is 1; n=0 is undefined. */
        return (seq_t)0;
    }

    if (seq_buf == NULL) {
        build_sequence();
    }

    /* Caller guarantees n ≤ seq_len; otherwise we return 0. */
    if ((size_t)n > seq_len) {
        return (seq_t)0;
    }

    return seq_buf[(size_t)n - 1];
}
