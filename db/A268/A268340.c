/*
 * A268340  Characteristic function of the prime powers p^k, k >= 2.
 * https://oeis.org/A268340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a prime power p^k with k ≥ 2, otherwise 0
 * Range:     u8 n <= 9911, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   build_spf, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* build_spf */
#include "../utils/free_cache.h" /* free_cache */

static idx_t *spf_cache = NULL;      /* smallest prime factor table */
static idx_t  spf_cap   = 0;          /* maximum n for which spf_cache is valid */

static void
ensure_spf(idx_t limit)
{
    if (limit <= spf_cap)
        return;

    /* allocate new table and free old one */
    idx_t *new_tbl;
    build_spf(limit, &new_tbl);       /* new_tbl[0..limit] */

    if (spf_cache != NULL) {
        free_cache((seq_t *)spf_cache);
    }

    spf_cache = new_tbl;
    spf_cap   = limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)     /* undefined for n=0, return 0 for safety */
        return 0;

    /* 1 is not a prime power with exponent ≥ 2 */
    if (n == 1)
        return 0;

    ensure_spf(n);

    idx_t  m  = n;
    idx_t  p  = spf_cache[m];
    if (p == m)          /* n is prime */
        return 0;

    /* count exponent of the smallest prime factor */
    int exp = 1;
    m /= p;
    while (m % p == 0) {
        exp++;
        m /= p;
    }

    return (m == 1 && exp >= 2) ? (seq_t)1 : (seq_t)0;
}
