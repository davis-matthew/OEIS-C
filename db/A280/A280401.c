/*
 * A280401  Number of 3Xn 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280401:
   Number of 3×n 0..2 arrays with no element unequal to a strict majority
   of its horizontal and antidiagonal neighbors, except exactly one element.
   The sequence satisfies the empirical linear recurrence
   for n > 19:
      a(n) =  7 a(n-1) -13 a(n-2) -13 a(n-3) +69 a(n-4)
             -51 a(n-5) -79 a(n-6) +121 a(n-7) + 8 a(n-8)
             -24 a(n-9) - 80 a(n-10) -24 a(n-11) +176 a(n-12)
             -32 a(n-13) -80 a(n-14) -48 a(n-15)          +64 a(n-17)
   with initial values (n = 1 … 19):
      a(1)=0 , a(2)=14 , a(3)=66 , a(4)=238 , a(5)=769 ,
      a(6)=2432 , a(7)=7440 , a(8)=22494 , a(9)=66934 , a(10)=197146 ,
      a(11)=575042 , a(12)=1664510 , a(13)=4785950 , a(14)=13683634 ,
      a(15)=38931434 , a(16)=110291246 , a(17)=311272806 ,
      a(18)=875559938 , a(19)=2455439762.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 .. 19 (0‑based indices 0 .. 18) */
    static const seq_t init[19] = {
        0ULL, 14ULL, 66ULL, 238ULL, 769ULL, 2432ULL, 7440ULL,
        22494ULL, 66934ULL, 197146ULL, 575042ULL, 1664510ULL,
        4785950ULL, 13683634ULL, 38931434ULL, 110291246ULL,
        311272806ULL, 875559938ULL, 2455439762ULL
    };

    /* Recurrence coefficients for offsets 1 .. 17 */
    static const int16_t coef[18] = {
        0,          /* unused 0‑th */
        7,          /* a(n-1)  */
        -13,        /* a(n-2)  */
        -13,        /* a(n-3)  */
        69,         /* a(n-4)  */
        -51,        /* a(n-5)  */
        -79,        /* a(n-6)  */
        121,        /* a(n-7)  */
        8,          /* a(n-8)  */
        -24,        /* a(n-9)  */
        -80,        /* a(n-10) */
        -24,        /* a(n-11) */
        176,        /* a(n-12) */
        -32,        /* a(n-13) */
        -80,        /* a(n-14) */
        -48,        /* a(n-15) */
        0,          /* a(n-16) – coefficient 0 */
        64          /* a(n-17) */
    };

    /* Static cache for memoised terms */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of terms currently cached */

    /* Initialise cache with the first 19 terms on first use */
    if (size == 0) {
        size = 19;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        for (idx_t i = 0; i < size; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if necessary */
    if (n > size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 1; j <= 17; ++j) {
                idx_t k = i - j;
                if (k < 1) continue;      /* reference term not yet defined */
                seq_t val = cache[k - 1];
                int16_t c = coef[j];
                if (c > 0)
                    sum_pos += (seq_t)c * val;
                else if (c < 0)
                    sum_neg += (seq_t)(-c) * val;
                /* else 0 – skip */
            }
            cache[i - 1] = sum_pos - sum_neg;
        }
        size = new_size;
    }

    return cache[n - 1];
}
