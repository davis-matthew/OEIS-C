/*
 * A297810  Number of nX4 0..1 arrays with every element equal to 2 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 2*a(n-2) + 3*a(n-4) + 2*a(n-5)
 *              - 4*a(n-6) - 11*a(n-7) - 11*a(n-8) - 6*a(n-9)
 *              + a(n-10) + 6*a(n-11) + 5*a(n-12) - a(n-14)
 *                for n > 21
 * Initial values (index from 1):
 *    a(1)=0, a(2)=1, a(3)=2, a(4)=8, a(5)=13, a(6)=27,
 *    a(7)=41, a(8)=100, a(9)=208, a(10)=450,
 *    a(11)=898, a(12)=1871, a(13)=3889, a(14)=8201,
 *    a(15)=17095, a(16)=35607, a(17)=74032,
 *    a(18)=154537, a(19)=322580, a(20)=673283,
 *    a(21)=1403752
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache        = NULL;
static idx_t  cache_cap    = 0;   /* allocated size          */
static idx_t  computed_up_to = 0; /* highest index computed  */

static const seq_t init[22] = {
    /* 0 */ 0,
    /* 1 */ 0,
    /* 2 */ 1,
    /* 3 */ 2,
    /* 4 */ 8,
    /* 5 */ 13,
    /* 6 */ 27,
    /* 7 */ 41,
    /* 8 */ 100,
    /* 9 */ 208,
    /*10 */ 450,
    /*11 */ 898,
    /*12 */ 1871,
    /*13 */ 3889,
    /*14 */ 8201,
    /*15 */ 17095,
    /*16 */ 35607,
    /*17 */ 74032,
    /*18 */ 154537,
    /*19 */ 322580,
    /*20 */ 673283,
    /*21 */ 1403752
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; return 0 for safety   */

    if (n <= 21) return init[(size_t)n];

    /* Grow cache if needed */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < n + 1) new_cap *= 2;

        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();    /* allocation failed */

        if (cache) {
            for (idx_t i = 0; i <= computed_up_to; ++i)
                new_cache[i] = cache[i];
        }

        free(cache);
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise cache[1..21] once */
    if (computed_up_to < 21) {
        for (idx_t i = 1; i <= 21; ++i)
            cache[i] = init[i];
        computed_up_to = 21;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += 2 * cache[i - 2];
        val += 3 * cache[i - 4];
        val += 2 * cache[i - 5];
        val -= 4 * cache[i - 6];
        val -= 11 * cache[i - 7];
        val -= 11 * cache[i - 8];
        val -= 6 * cache[i - 9];
        val += cache[i - 10];
        val += 6 * cache[i - 11];
        val += 5 * cache[i - 12];
        val -= cache[i - 14];
        cache[i] = val;
    }
    computed_up_to = n;

    return cache[(size_t)n];
}
