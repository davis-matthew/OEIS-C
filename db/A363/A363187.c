/*
 * A363187  Prime numbers that are the average of three consecutive odd
 *          semiprimes.
 * https://oeis.org/A363187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 286, u32 n <= 287, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers that are the average of three consecutive odd semiprimes */
/* a(n) = the n-th prime p such that there exist odd semiprimes x<y<z with (x+y+z)/3 = p */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime_big.h"

#define MAX_CACHE 15000   /* sufficient for n up to ~10000 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* cache for sequence terms */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;

    /* state for scanning odd numbers */
    static idx_t cur = 9;               /* next odd number to test       */
    static idx_t last[3] = {0, 0, 0};   /* last three odd semiprimes      */
    static size_t semiprime_seen = 0;   /* how many odd semiprimes seen  */

    /* Small‑prime‑factor table */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;         /* largest n for which SPF is built */

    /* ensure we have enough terms in the cache */
    while (cache_len < n)
    {
        /* Ensure SPF table covers the current odd number cur */
        if (cur > spf_limit)
        {
            idx_t new_limit = cur * 2;
            if (spf) free(spf);
            build_spf(new_limit, &spf);
            spf_limit = new_limit;
        }

        /* Test whether cur is an odd semiprime */
        if (cur & 1 && spf[cur] < cur)   /* composite and odd */
        {
            idx_t f1 = spf[cur];
            idx_t f2 = cur / f1;
            if (f2 >= f1 && spf[f2] == f2)   /* both factors prime */
            {
                /* shift the circular buffer */
                last[2] = last[1];
                last[1] = last[0];
                last[0] = cur;
                semiprime_seen++;

                if (semiprime_seen >= 3)
                {
                    /* compute the average using seq_t arithmetic */
                    seq_t sum = last[0] + last[1] + last[2];
                    if (sum % 3 == 0)
                    {
                        seq_t avg = sum / 3;
                        if (is_prime_big(avg))
                        {
                            if (cache_len < MAX_CACHE)
                                cache[cache_len++] = avg;
                            else
                                /* escalation not required for supported n range */
                                break;
                        }
                    }
                }
            }
        }

        cur += 2;   /* next odd number */
    }

    return cache[n - 1];
}
