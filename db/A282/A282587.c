/*
 * A282587  Number of nX2 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -24*a(n-2) +15*a(n-3) -36*a(n-4) +195*a(n-5)
 *            -143*a(n-6) -6*a(n-7) -492*a(n-8) +488*a(n-9) +96*a(n-10)
 *            +384*a(n-11) -512*a(n-12) for n>12 with initial terms: a(1)=0,
 *            a(2)=0, a(3)=4, a(4)=12, a(5)=52, a(6)=220, a(7)=916, a(8)=3700,
 *            a(9)=14752, a(10)=58156, a(11)=225304, a(12)=862968
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for realloc, NULL */
#include <stddef.h>  /* for size_t */

static seq_t *cache = NULL;        /* 1‑based cache: cache[n] = a(n) */
static size_t cache_len = 0;       /* largest n for which cache is defined */

/* compute a(n) with memoization and recurrence */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, safe fallback */

    /* expand cache if needed */
    if (!cache || cache_len < n) {
        /* allocate space for indices 0..n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure -> 0 */
        cache = tmp;

        /* initialise new entries */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            seq_t val;
            if (i <= 12) {            /* initial values */
                switch ((unsigned)i) {
                    case 1: val = (seq_t)0;  break;
                    case 2: val = (seq_t)0;  break;
                    case 3: val = (seq_t)4;  break;
                    case 4: val = (seq_t)12; break;
                    case 5: val = (seq_t)52; break;
                    case 6: val = (seq_t)220;break;
                    case 7: val = (seq_t)916;break;
                    case 8: val = (seq_t)3700;break;
                    case 9: val = (seq_t)14752;break;
                    case 10: val = (seq_t)58156;break;
                    case 11: val = (seq_t)225304;break;
                    case 12: val = (seq_t)862968;break;
                    default: val = (seq_t)0;  /* unreachable */
                }
            } else {                    /* recurrence */
                val  = (seq_t)9  * cache[i-1];
                val -= (seq_t)24 * cache[i-2];
                val += (seq_t)15 * cache[i-3];
                val -= (seq_t)36 * cache[i-4];
                val += (seq_t)195 * cache[i-5];
                val -= (seq_t)143 * cache[i-6];
                val -= (seq_t)6   * cache[i-7];
                val -= (seq_t)492 * cache[i-8];
                val += (seq_t)488 * cache[i-9];
                val += (seq_t)96  * cache[i-10];
                val += (seq_t)384 * cache[i-11];
                val -= (seq_t)512 * cache[i-12];
            }
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }
    return cache[n];
}
