/*
 * A290895  p-INVERT of the positive integers, where p(S) = 1 - S^7.
 * https://oeis.org/A290895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 91*a(n-2) + 364*a(n-3) - 1001*a(n-4) +
 *            2002*a(n-5) - * 3003*a(n-6) + 3433*a(n-7) - 3003*a(n-8) +
 *            2002*a(n-9) - 1001*a(n-10) + 364*a(n-11) - * 91*a(n-12) +
 *            14*a(n-13) - a(n-14) * with initial values: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, * a(7)=14, a(8)=105,
 *            a(...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 25, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_capacity(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Allocation failure – abort */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

static void init_cache(void) {
    if (cache_len > 0)
        return;
    ensure_cache_capacity(15);
    /* fill first 15 values */
    cache[0]  = (seq_t)0;
    cache[1]  = (seq_t)0;
    cache[2]  = (seq_t)0;
    cache[3]  = (seq_t)0;
    cache[4]  = (seq_t)0;
    cache[5]  = (seq_t)0;
    cache[6]  = (seq_t)1;
    cache[7]  = (seq_t)14;
    cache[8]  = (seq_t)105;
    cache[9]  = (seq_t)560;
    cache[10] = (seq_t)2380;
    cache[11] = (seq_t)8568;
    cache[12] = (seq_t)27132;
    cache[13] = (seq_t)77521;
    cache[14] = (seq_t)203518;
    cache_len = 15;
}

seq_t a(idx_t n) {
    init_cache();
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    size_t target = (size_t)n;
    if (target < cache_len)
        return cache[cache_len - 1];

    ensure_cache_capacity(target + 1);

    for (size_t i = cache_len; i <= target; ++i) {
        /* compute a(i) using recurrence */
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        seq_t t6 = cache[i-6];
        seq_t t7 = cache[i-7];
        seq_t t8 = cache[i-8];
        seq_t t9 = cache[i-9];
        seq_t t10 = cache[i-10];
        seq_t t11 = cache[i-11];
        seq_t t12 = cache[i-12];
        seq_t t13 = cache[i-13];
        seq_t t14 = cache[i-14];

        seq_t sum_pos = (seq_t)14 * t1
                      + (seq_t)364 * t3
                      + (seq_t)2002 * t5
                      + (seq_t)3433 * t7
                      + (seq_t)2002 * t9
                      + (seq_t)364 * t11
                      + (seq_t)14   * t13;

        seq_t sum_neg = (seq_t)91 * t2
                      + (seq_t)1001 * t4
                      + (seq_t)3003 * t6
                      + (seq_t)3003 * t8
                      + (seq_t)1001 * t10
                      + (seq_t)91   * t12
                      + (seq_t)1    * t14;

        cache[i] = sum_pos - sum_neg;
    }
    cache_len = target + 1;
    return cache[target];
}
