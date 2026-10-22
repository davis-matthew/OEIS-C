/*
 * A303803  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) -3*a(n-3) +6*a(n-4) -5*a(n-5) -2*a(n-6)
 *            - a(n-7) -6*a(n-8) +4*a(n-9) +2*a(n-10) Initial values: a(1)=3,
 *            a(2)=7, a(3)=9, a(4)=15, a(5)=27, a(6)=47, a(7)=83, a(8)=161,
 *            a(9)=321, a(10)=641
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   mul_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_int.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms stored (>=0) */
static idx_t cache_capacity = 0; /* allocated capacity */

/* grow the cache to hold at least n terms (n>=1) */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* enlarge the buffer if necessary */
    if (cache_capacity < n) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();    /* out of memory */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* initialise the first ten terms once */
    if (cache_size == 0) {
        cache[0] = (seq_t)3;   /* a(1) */
        cache[1] = (seq_t)7;   /* a(2) */
        cache[2] = (seq_t)9;   /* a(3) */
        cache[3] = (seq_t)15;  /* a(4) */
        cache[4] = (seq_t)27;  /* a(5) */
        cache[5] = (seq_t)47;  /* a(6) */
        cache[6] = (seq_t)83;  /* a(7) */
        cache[7] = (seq_t)161; /* a(8) */
        cache[8] = (seq_t)321; /* a(9) */
        cache[9] = (seq_t)641; /* a(10) */
        cache_size = 10;
    }

    /* compute remaining terms up to n */
    while (cache_size < n) {
        idx_t i = cache_size; /* zero‑based index, a(i+1) */

        seq_t p1 = cache[i - 1];
        seq_t p2 = cache[i - 2];
        seq_t p3 = cache[i - 3];
        seq_t p4 = cache[i - 4];
        seq_t p5 = cache[i - 5];
        seq_t p6 = cache[i - 6];
        seq_t p7 = cache[i - 7];
        seq_t p8 = cache[i - 8];
        seq_t p9 = cache[i - 9];
        seq_t p10 = cache[i - 10];

        /* positive part of the recurrence */
        seq_t pos = mul_int(p1, 3);
        pos += mul_int(p4, 6);
        pos += mul_int(p9, 4);
        pos += mul_int(p10, 2);

        /* negative part */
        seq_t neg = p2;
        neg += mul_int(p3, 3);
        neg += mul_int(p5, 5);
        neg += mul_int(p6, 2);
        neg += p7;
        neg += mul_int(p8, 6);

        /* result for a(i+1) */
        seq_t val = pos - neg;  /* recurrence guarantees val>0 */
        cache[cache_size] = val;
        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence is undefined for n=0 */

    ensure_cache(n);
    return cache[n - 1];
}
