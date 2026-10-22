/*
 * A326824  a(1) = 1; a(n) = Sum_{d|n, d < n} phi(n/d) * d * a(d).
 * https://oeis.org/A326824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A326824
 *  a(1) = 1;
 *  a(n) = Sum_{d|n, d < n} phi(n/d) * d * a(d)     (n >= 2)
 *  a(n) = Sum_{k=1..n-1} gcd(n,k) * a(gcd(n,k))   (alternative form)
 *
 *  The implementation uses memoisation in a static array of size 200000
 *  (enough for n up to about 10000 as required).  All values are stored
 *  in seq_t.  seq_t and idx_t are supplied by the build system.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/calc_phi.h"

/* static cache ---------------------------------------------------------- */
#define CACHE_SIZE 200000
static seq_t  cache[CACHE_SIZE];
static bool   cache_initialised = false;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise cache once */
    if (!cache_initialised) {
        for (size_t i = 0; i < CACHE_SIZE; ++i)
            cache[i] = 0;
        cache[1] = 1;                 /* a(1) = 1 */
        cache_initialised = true;
    }

    /* return already computed value (zero means "not yet computed") */
    if (n < CACHE_SIZE && cache[n] != 0)
        return cache[n];

    /* base case */
    if (n == 1)
        return 1;

    /* compute a(n) ----------------------------------------------- */
    seq_t sum = 0;

    /* divisor d = 1 */
    {
        seq_t phi_n = calc_phi((seq_t)n);          /* phi(n) */
        sum += phi_n * (seq_t)1 * cache[1];        /* a(1) == 1 */
    }

    /* other divisors d, d < n */
    for (idx_t d = 2; (d * d) <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t phi_nd = calc_phi((seq_t)(n / d));
            if (cache[d] == 0)                 /* recursive memoisation */
                a(d);
            sum += phi_nd * (seq_t)d * cache[d];

            /* paired divisor n/d, distinct from d */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t phi_nd2 = calc_phi((seq_t)(n / d2));   /* phi(d)  */
                if (cache[d2] == 0)
                    a(d2);
                sum += phi_nd2 * (seq_t)d2 * cache[d2];
            }
        }
    }

    /* store and return */
    if (n < CACHE_SIZE)
        cache[n] = sum;
    return sum;
}
