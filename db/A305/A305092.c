/*
 * A305092  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2)+11*a(n-3)+7*a(n-4)-16*a(n-5)-46*a(n-6)-31*a(n-7)
 *            +27*a(n-8)+64*a(n-9)+38*a(n-10)-20*a(n-11)-25*a(n-12)
 *            -6*a(n-13)+7*a(n-14)+a(n-15)-2*a(n-16) (n>17) Initial values:
 *            a(1)=1, a(2)=9, a(3)=14, a(4)=17, a(5)=64, a(6)=158, a(7)=229,
 *            a(8)=630, a(9)=1626, a(10)=2950, a(11)=6627, a(12)=16...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache = NULL;             /* dynamic cache for sequence values   */
static idx_t  computed_max = 0;          /* highest index computed so far      */
static idx_t  cache_size   = 0;          /* allocated size of cache array      */
static const idx_t MIN_CACHE = 10001;    /* initial allocation size (for n<=10k) */

static void init_cache(idx_t n_needed)
{
    if (cache == NULL) {
        cache_size = (n_needed + 1 > MIN_CACHE ? n_needed + 1 : MIN_CACHE);
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        /* set the first 17 terms (a(0) unused) */
        cache[1]  = 1;
        cache[2]  = 9;
        cache[3]  = 14;
        cache[4]  = 17;
        cache[5]  = 64;
        cache[6]  = 158;
        cache[7]  = 229;
        cache[8]  = 630;
        cache[9]  = 1626;
        cache[10] = 2950;
        cache[11] = 6627;
        cache[12] = 16872;
        cache[13] = 34743;
        cache[14] = 73749;
        cache[15] = 177939;
        cache[16] = 391569;
        cache[17] = 832239;
        computed_max = 17;
    } else if (n_needed + 1 > cache_size) {
        /* enlarge the cache to at least n_needed+1 elements */
        idx_t new_size = n_needed + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (new_size > cache_size) {
            memset(cache + cache_size, 0,
                   (size_t)(new_size - cache_size) * sizeof(seq_t));
        }
        cache_size = new_size;
    }
}

static inline seq_t recurrence(idx_t n)
{
    seq_t v = 0;
    v += (seq_t)2  * cache[n-2];
    v += (seq_t)11 * cache[n-3];
    v += (seq_t)7  * cache[n-4];
    v -= (seq_t)16 * cache[n-5];
    v -= (seq_t)46 * cache[n-6];
    v -= (seq_t)31 * cache[n-7];
    v += (seq_t)27 * cache[n-8];
    v += (seq_t)64 * cache[n-9];
    v += (seq_t)38 * cache[n-10];
    v -= (seq_t)20 * cache[n-11];
    v -= (seq_t)25 * cache[n-12];
    v -= (seq_t)6  * cache[n-13];
    v += (seq_t)7  * cache[n-14];
    v +=        cache[n-15];
    v -= (seq_t)2  * cache[n-16];
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is immaterial */
    init_cache(n);
    if (n <= computed_max) {
        return cache[n];
    }
    /* compute terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 17) continue;           /* already initialized */
        cache[i] = recurrence(i);
    }
    computed_max = n;
    return cache[n];
}
