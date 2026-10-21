/*
 * A246541  Take the squares of all P_(n+2)-rough numbers less than the
 *          (n+1)-th primorial and mod each by the (n+1)-th primorial. There
 *          will be a(n) different results.
 * https://oeis.org/A246541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=1..n} (prime(j+1)-1) / 2^n Simplification: a(n) =
 *            ∏_{i=2}^{n+1} ((prime(i)-1)/2) The product runs over all primes
 *            except 2. All operations use only integer arithmetic suitable
 *            for any seq_t supported by the build
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static storage for primes */
static uint64_t *primes = NULL;     /* primes[0]=2, primes[1]=3, ... */
static size_t primes_cnt = 0;       /* number of primes stored */

/* Ensure at least 'need' primes are available (i.e. primes[0..need-1]). */
static void ensure_primes(idx_t need)
{
    if (need <= primes_cnt)
        return;

    /* estimate an upper bound for the required prime.
       For n>=2, p_n < 15*n is safe for n up to >10^5. */
    idx_t limit = (need > 2) ? (15 * need + 100) : 15;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;

    /* first attempt */
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* if not enough primes, double the limit until satisfied */
    while (!new_primes || new_cnt < need) {
        limit *= 2;
        free(new_primes);
        build_primes_upto(limit, &new_primes, &new_cnt);
        if (!new_primes) break;
    }

    if (new_primes && new_cnt >= need) {
        free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
    } else {
        /* allocation failed – keep old array (if any) */
        free(new_primes);
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for offset < 1 */

    /* we need primes[0..n] (i.e. up to the (n+1)-th prime). */
    idx_t need = n + 1;
    ensure_primes(need);
    if (primes_cnt < (size_t)need)
        return 0;                       /* allocation failure */

    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= need; ++i) {
        /* primes[i-1] is the i-th prime. Compute (p-1)/2. */
        seq_t factor = (seq_t)((primes[i-1] - 1) / 2);
        prod *= factor;
    }
    return prod;
}
