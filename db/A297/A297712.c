/*
 * A297712  Number of irredundant sets in the n-dipyramidal graph.
 * https://oeis.org/A297712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A297712: Number of irredundant sets in the n‑dipyramidal graph.
    Recurrence for n > 11:
      a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 2*a(n-4) - 2*a(n-5)
             + 2*a(n-7) - a(n-8)
    Initial values (offset 1):
      a(1)=7, a(2)=22, a(3)=29, a(4)=39, a(5)=60, a(6)=94,
      a(7)=151, a(8)=241, a(9)=400, a(10)=671, a(11)=1135,
      a(12)=1924
*/
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms, 0‑based index a(n-1) */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static size_t cache_len = 0;  /* number of terms currently stored */

/* Ensure the cache is initialized with the 12 known seed values. */
static void init_base(void)
{
    if (cache_len > 0)
        return;

    size_t needed = 12;
    if (cache_cap < needed) {
        size_t newcap = needed > 8 ? needed : 8;
        seq_t *tmp = NULL;
        if (cache)
            tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        else
            tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) /* out of memory : terminate program */
            exit(1);
        cache = tmp;
        cache_cap = newcap;
    }

    cache[0] = (seq_t)7;
    cache[1] = (seq_t)22;
    cache[2] = (seq_t)29;
    cache[3] = (seq_t)39;
    cache[4] = (seq_t)60;
    cache[5] = (seq_t)94;
    cache[6] = (seq_t)151;
    cache[7] = (seq_t)241;
    cache[8] = (seq_t)400;
    cache[9] = (seq_t)671;
    cache[10] = (seq_t)1135;
    cache[11] = (seq_t)1924;
    cache_len = 12;
}

/* Ensure the cache can hold at least n terms. */
static void expand_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 8;
    if (newcap < (size_t)n)
        newcap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) /* out of memory */
        exit(1);
    cache = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; behaviour for n<1 is irrelevant */

    init_base();

    /* compute values up to index n-1 if needed */
    if ((size_t)n > cache_len) {
        expand_cache(n);
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* recurrence uses indexes i-1 ... i-8, all valid because i>=12 at this point */
            seq_t val = 0;
            val += (seq_t)3 * cache[i - 1];
            val -= (seq_t)2 * cache[i - 2];
            val -= cache[i - 3];
            val += (seq_t)2 * cache[i - 4];
            val -= (seq_t)2 * cache[i - 5];
            val += (seq_t)2 * cache[i - 7];
            val -= cache[i - 8];
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }
    return cache[n - 1];
}
