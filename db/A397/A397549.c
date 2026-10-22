/*
 * A397549  Multiplicative with a(p^e) = (e+1)^2*(e+2)/2.
 * https://oeis.org/A397549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Multiplicative with a(p^e) = (e+1)^2*(e+2)/2
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* static cache for smallest prime factor array up to the latest n */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    /* rebuild spf array if needed */
    if (n > spf_limit)
    {
        /* free previous array if any */
        if (spf != NULL)
            free(spf);
        /* build new spf up to n */
        idx_t *new_spf = NULL;
        build_spf(n, &new_spf);
        spf = new_spf;
        spf_limit = n;
    }

    /* factor n using the spf array */
    idx_t temp = n;
    seq_t result = 1;
    while (temp > 1)
    {
        uint64_t p = spf[temp];
        int e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            e++;
        }
        /* compute (e+1)^2*(e+2)/2 in seq_t */
        seq_t ee = (seq_t)e;
        seq_t tmp = ee + 1;      /* e+1 */
        tmp = tmp * tmp;         /* (e+1)^2 */
        tmp = tmp * (ee + 2);    /* (e+1)^2*(e+2) */
        tmp = tmp / 2;           /* divide by 2 */
        result = result * tmp;   /* multiply contributions */
    }
    return result;
}
