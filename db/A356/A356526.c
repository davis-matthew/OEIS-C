/*
 * A356526  Decimal expansion of number of millimeters of mercury (mmHg) in 1
 *          standard atmosphere (atm).
 * https://oeis.org/A356526
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of
 *            101325/133.322387415 with offset 3, i.e. a(3) = first digit (7
 *            of 759)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *digit_cache   = NULL;   /* cache of computed digits (starting at a(3)) */
static size_t cache_len     = 0;      /* number of digits currently in cache      */
static size_t cache_cap     = 0;      /* allocated capacity                       */
static unsigned long long decimal_remainder = 0;   /* remainder for decimal generation   */
static int initialized       = 0;      /* flag for first call                     */

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* undefined for n<3                         */

    size_t pos = (size_t)(n - 3);            /* position in cache for a(n)               */

    if (!initialized) {                      /* first time: compute integer part          */
        const unsigned long long NUM   = 101325ULL * 1000000000ULL; /* 1e9 scaling   */
        const unsigned long long DEN   = 133322387415ULL;

        unsigned long long q = NUM / DEN;    /* integer part                           */
        unsigned long long r = NUM % DEN;    /* initial remainder for decimal digits     */

        /* extract digits of q into a temporary array */
        unsigned int tmp[20];
        size_t cnt = 0;
        unsigned long long t = q;
        while (t > 0) {
            tmp[cnt++] = (unsigned int)(t % 10);
            t /= 10;
        }
        /* reverse to natural order */
        for (size_t i = 0; i < cnt / 2; ++i) {
            unsigned int swap = tmp[i];
            tmp[i] = tmp[cnt - 1 - i];
            tmp[cnt - 1 - i] = swap;
        }

        /* allocate cache for integer digits (at least cnt) */
        cache_cap = cnt > 16 ? cnt : 16;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        for (size_t i = 0; i < cnt; ++i)
            digit_cache[i] = (seq_t)tmp[i];

        cache_len = cnt;
        decimal_remainder = r;
        initialized = 1;
    }

    /* generate missing decimal digits up to position pos */
    if (pos >= cache_len) {
        /* ensure enough capacity */
        if (cache_cap == 0) {
            cache_cap = 16;
            digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len <= pos) {
            if (cache_len >= cache_cap) {               /* grow cache if needed */
                cache_cap *= 2;
                digit_cache = (seq_t *)realloc(digit_cache,
                                               cache_cap * sizeof(seq_t));
            }

            /* compute next decimal digit */
            unsigned long long mult = decimal_remainder * 10ULL;
            unsigned long long digit = mult / 133322387415ULL;
            decimal_remainder = mult % 133322387415ULL;

            digit_cache[cache_len] = (seq_t)digit;
            ++cache_len;
        }
    }

    return digit_cache[pos];
}
