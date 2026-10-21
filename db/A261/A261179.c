/*
 * A261179  a(n) is the prime(n)-th term of Stern-Brocot sequence, A002487.
 * https://oeis.org/A261179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = A002487(prime(n))
 * Range:     u8 n <= 633, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static storage for primes and Stern sequence cache */
    static uint64_t *primes      = NULL;
    static size_t  primes_cnt    = 0;
    static idx_t   primes_limit  = 0;

    static seq_t *stern         = NULL;
    static size_t  stern_cap     = 0;
    static size_t  stern_size    = 0;   /* number of computed Stern terms */

    /* Ensure we have at least n primes */
    if (n > 0 && primes_cnt < (size_t)n) {
        idx_t limit = (primes_limit < 16) ? 16 : primes_limit * 2;
        while (true) {
            uint64_t *tmp   = NULL;
            size_t    tmp_cnt = 0;
            build_primes_upto(limit, &tmp, &tmp_cnt);
            free(primes);
            primes = tmp;
            primes_cnt = tmp_cnt;
            primes_limit = limit;
            if (primes_cnt >= (size_t)n) break;
            limit *= 2;
        }
    }

    uint64_t p = primes[n-1];   /* nth prime (1‑based) */

    /* Expand Stern cache if necessary */
    if (stern_size <= p) {
        idx_t need = p + 1;
        if (stern_cap < need) {
            ensure_capacity(need, &stern_cap, (void **)&stern, sizeof(seq_t));
        }

        /* Initialise base of Stern sequence if it hasn't been done yet */
        if (stern_size == 0) {
            stern[0] = (seq_t)0;
            stern[1] = (seq_t)1;
            stern_size = 2;
        }

        /* Compute missing Stern terms up to index p */
        for (idx_t i = stern_size; i <= p; ++i) {
            if ((i & (idx_t)1) == 0) {               /* even */
                stern[i] = stern[i >> 1];
            } else {                                 /* odd */
                stern[i] = stern[i >> 1] + stern[(i >> 1) + 1];
            }
        }
        stern_size = p + 1;
    }

    return stern[p];
}
