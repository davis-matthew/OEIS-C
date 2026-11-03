/* build_spf: a smallest-prime-factor table.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BUILD_SPF_H
#define OEISC_UTILS_BUILD_SPF_H
#include <stdint.h>
#include <stdlib.h>

/* Build the smallest‑prime‑factor table up to limit. */
static void build_spf(idx_t limit, idx_t **spf_out)
{
    idx_t i, j;
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) {
        *spf_out = NULL;
        return;
    }

    for (i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    for (i = 2; i <= limit; ++i) {
        if (!spf[i]) {            /* i is prime */
            spf[i] = i;
            if (i <= limit / i)   /* avoid overflow in j loop */
                for (j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = i;
        }
    }
    *spf_out = spf;
}

#endif
