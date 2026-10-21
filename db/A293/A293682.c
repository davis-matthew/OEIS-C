/*
 * A293682  a(n) = least odd number k > 1 such that p = prime(n) is the middle
 *          of k consecutive primes which have arithmetic mean p, or 1 if no
 *          such k exists.
 * https://oeis.org/A293682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest odd window size k > 1 such that prime(n)
   lies at the center of k consecutive primes with arithmetic mean
   prime(n).  If no such window exists, return 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"  /* declares build_primes_upto */

static uint64_t *primes = NULL;        /* array of all primes up to limit */
static size_t prime_cnt = 0;           /* number of primes stored          */
static unsigned long long *pref = NULL;/* prefix sums of primes            */

/* one‑time initialization */
static void init_primes(void)
{
    if (primes != NULL)
        return;                       /* already initialized */

    const idx_t LIMIT = 2000000;       /* sieve limit – plenty for n ≤ 10 000 */
    build_primes_upto(LIMIT, &primes, &prime_cnt);

    pref = (unsigned long long *)malloc((prime_cnt + 1) * sizeof(unsigned long long));
    pref[0] = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        pref[i + 1] = pref[i] + primes[i];
}

/* Compute a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    init_primes();

    if (n == 0)      /* undefined, but guard against 0 */
        return 0;

    if ((idx_t)prime_cnt < n)
        return 1;    /* requested prime beyond pre‑computed range */

    /* prime(n) is at zero‑based index n-1 */
    const size_t idx = (size_t)(n - 1);
    const seq_t center = (seq_t)primes[idx];

    /* Search odd window sizes k = 3,5,7,… (m = (k-1)/2) */
    for (size_t m = 1;; ++m) {
        if (idx < m)                /* left bound would be negative */
            break;
        size_t left  = idx - m;
        size_t right = idx + m;
        if (right >= prime_cnt)      /* right bound would exceed array */
            break;

        unsigned long long sum = pref[right + 1] - pref[left];
        unsigned long long k   = 2 * (unsigned long long)m + 1;

        if (sum == center * k)
            return (seq_t)k;        /* first suitable odd k found */
    }

    return 1;                        /* no valid window */
}
