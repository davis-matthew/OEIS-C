/*
 * A246929  a(n) = prime(11*n).
 * https://oeis.org/A246929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(11*n)
 * Range:     u8 n <= 4, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; never used for n<1 */

    idx_t target = 11 * n;           /* position of the prime (1‑based) */

    /* lazy static cache of primes up to some limit */
    static uint64_t *primes   = NULL;
    static size_t   primes_cnt = 0;
    static idx_t    primes_lim = 0;

    if (primes == NULL || target > primes_cnt) {
        /* Choose an upper bound that comfortably contains p_target.
           For n≤10 000 (target≤110 000) the bound target*20+100
           exceeds the 110 000‑th prime. */
        idx_t limit = target * 20 + 100;
        if (limit < 100) limit = 100;

        size_t  cnt;
        uint64_t *list;
        build_primes_upto(limit, &list, &cnt);

        if (primes) free(primes);          /* dispose old list */
        primes     = list;
        primes_cnt = cnt;
        primes_lim = limit;
    }

    /* primes[] is 0‑based; target is 1‑based */
    return (seq_t)primes[target - 1];
}
