/*
 * A248591  Numerators of the (simplified) rationals n*2^(n - 1)/(n - 1)! .
 * https://oeis.org/A248591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator(n*2^(n-1)/(n-1)!)
 * Range:     u8 n <= 15, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;    /* primes <= current_limit */
static size_t primes_count = 0;    /* number of primes stored */
static idx_t primes_limit = 0;     /* largest n for which primes are generated */

/* Ensure we have all primes up to n (inclusive) */
static void ensure_primes_up_to(idx_t n) {
    if (n <= primes_limit) return;
    uint64_t *new_primes = NULL;
    size_t new_count = 0;
    build_primes_upto(n, &new_primes, &new_count);
    if (primes) free(primes);
    primes = new_primes;
    primes_count = new_count;
    primes_limit = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n=0, but return 0 */

    ensure_primes_up_to(n);

    seq_t numer = 1;                    /* numerator of the reduced fraction */

    /* Loop over all primes <= n */
    for (size_t idx = 0; idx < primes_count && primes[idx] <= n; ++idx) {
        idx_t p = primes[idx];

        /* exponent of p in the numerator: from n and from 2^(n-1) */
        idx_t exp_num = 0;
        if (p == 2) exp_num = (n - 1);        /* from 2^(n-1) */
        idx_t tmp = n;
        while (tmp % p == 0) {                /* factor from n itself */
            ++exp_num;
            tmp /= p;
        }

        /* exponent of p in (n-1)! */
        idx_t exp_den = 0;
        idx_t m = n - 1;
        while (m) {
            m /= p;
            exp_den += m;
        }

        /* exponent remaining in the numerator after cancellation */
        idx_t ex = exp_num > exp_den ? exp_num - exp_den : 0;
        if (ex == 0) continue;

        /* multiply by p^ex */
        seq_t pow = 1;
        for (idx_t i = 0; i < ex; ++i) {
            pow *= (seq_t)p;
        }
        numer *= pow;
    }

    return numer;
}
