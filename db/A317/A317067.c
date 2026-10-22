/*
 * A317067  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) +9*a(n-2) +8*a(n-3) +a(n-4) -5*a(n-5) -2*a(n-6)
 *            -a(n-7) -a(n-8) Initial values: a(1)=1, a(2)=25, a(3)=153,
 *            a(4)=1161, a(5)=8529, a(6)=62866, a(7)=463269, a(8)=3413985
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Pointer to cached values; index 0 is unused. */
static seq_t *cache = NULL;
/* Number of allocated slots (including the unused 0 index). */
static size_t cap = 0;
/* Highest index that has been computed (>=0). */
static size_t computed = 0;

static void ensure_capacity(size_t new_cap)
{
    if (cap >= new_cap)
        return;
    size_t new_size = cap ? cap << 1 : 16;
    while (new_size < new_cap)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cap = new_size;
}

static void init_cache(void)
{
    ensure_capacity(9);          /* need indices 1..8 */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)25;
    cache[3] = (seq_t)153;
    cache[4] = (seq_t)1161;
    cache[5] = (seq_t)8529;
    cache[6] = (seq_t)62866;
    cache[7] = (seq_t)463269;
    cache[8] = (seq_t)3413985;
    computed = 8;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but defensive */

    if (cache == NULL)
        init_cache();

    size_t idx = (size_t)n;
    if (idx <= computed)
        return cache[idx];

    ensure_capacity(idx + 1);   /* make space for idx */

    /* Compute missing terms up to idx using the recurrence. */
    for (size_t i = computed + 1; i <= idx; ++i)
    {
#if defined(__cplusplus)
        /* C++ build: assume operator overloads for seq_t. */
        cache[i] = cache[i-1]*6 + cache[i-2]*9 + cache[i-3]*8 + cache[i-4]
                 - (cache[i-5]*5 + cache[i-6]*2 + cache[i-7] + cache[i-8]);
#else
        /* C build: use unsigned 128‑bit intermediates if available. */
#if defined(__SIZEOF_INT128__)
        typedef unsigned __int128 u128;
#else
        /* Fallback: use seq_t, overflow is modulo which matches unsigned behavior. */
        typedef seq_t u128;
#endif
        u128 t1 = (u128)6  * cache[i-1]
                + (u128)9  * cache[i-2]
                + (u128)8  * cache[i-3]
                + (u128)cache[i-4];
        u128 t2 = (u128)5  * cache[i-5]
                + (u128)2  * cache[i-6]
                + (u128)cache[i-7]
                + (u128)cache[i-8];
        cache[i] = (seq_t)(t1 - t2);
#endif
    }

    computed = idx;
    return cache[idx];
}
