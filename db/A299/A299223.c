/*
 * A299223  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A299223
   Recurrence (for n>16):
   a(n) = 5*a(n-1)+3*a(n-2)+16*a(n-3)-7*a(n-4)-32*a(n-5)-25*a(n-6)
          -62*a(n-7)+44*a(n-8)-57*a(n-9)-12*a(n-10)-74*a(n-11)
          +46*a(n-12)-20*a(n-13)-16*a(n-14)
   Initial values:
   a(1)=1, a(2)=17, a(3)=113, a(4)=628, a(5)=3669,
   a(6)=21792, a(7)=128610, a(8)=758715, a(9)=4478515,
   a(10)=26434415, a(11)=156022822, a(12)=920895737,
   a(13)=5435426140, a(14)=32081622538, a(15)=189355989729,
   a(16)=1117639651783. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, but harmless */

    static seq_t *cache = NULL;
    static idx_t sz   = 0;      /* number of terms currently stored */

    /* Grow the cache if needed */
    if (n > sz) {
        idx_t new_sz = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        cache = tmp;

        /* Initialise the first 16 terms (or fewer if n<16) */
        if (sz < 16) {
            static const seq_t init[16] = {
                1ULL,
                17ULL,
                113ULL,
                628ULL,
                3669ULL,
                21792ULL,
                128610ULL,
                758715ULL,
                4478515ULL,
                26434415ULL,
                156022822ULL,
                920895737ULL,
                5435426140ULL,
                32081622538ULL,
                189355989729ULL,
                1117639651783ULL
            };
            idx_t limit = (new_sz < 16 ? new_sz : 16);
            for (idx_t i = sz; i < limit; ++i)
                cache[i] = init[i];
        }

        /* Compute remaining terms using the recurrence */
        if (new_sz > 16) {
            idx_t start = (sz < 16) ? 17 : sz + 1;
            for (idx_t i = start; i <= new_sz; ++i) {
                /* a(i) = ... expressed in terms of cache indices (0‑based) */
                cache[i-1] =
                    5*cache[i-2] + 3*cache[i-3] + 16*cache[i-4]
                  - 7*cache[i-5] - 32*cache[i-6] - 25*cache[i-7]
                  - 62*cache[i-8] + 44*cache[i-9]
                  - 57*cache[i-10] - 12*cache[i-11] - 74*cache[i-12]
                  + 46*cache[i-13] - 20*cache[i-14] - 16*cache[i-15];
            }
        }

        sz = new_sz;
    }

    return cache[n-1];
}
