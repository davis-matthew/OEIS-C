/* build_primes: the primes up to a limit.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BUILD_PRIMES_H
#define OEISC_UTILS_BUILD_PRIMES_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Build all primes up to `limit` (inclusive) using a simple sieve.  */
static void build_primes(idx_t limit, idx_t **out, size_t *cnt)
{
    if (limit < 2) {                       /* no primes */
        *out = NULL;
        *cnt = 0;
        return;
    }

    unsigned char *mark = (unsigned char *)calloc((size_t)(limit + 1), 1);
    if (!mark) {                           /* allocation failure */
        *out = NULL;
        *cnt = 0;
        return;
    }

    size_t cap = limit / 10 + 1;            /* initial capacity guess  */
    idx_t *pr  = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t pc  = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            /* grow array if necessary */
            if (pc == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(pr, cap * sizeof(idx_t));
                if (!tmp) {               /* allocation failure */
                    free(mark);
                    free(pr);
                    *out = NULL;
                    *cnt = 0;
                    return;
                }
                pr = tmp;
            }
            pr[pc++] = i;

            /* mark multiples of i */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    mark[j] = 1;
            }
        }
    }
    free(mark);

    *out = pr;
    *cnt = pc;
}

#endif
