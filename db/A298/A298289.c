/*
 * A298289  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - 11*a(n-4) + 4*a(n-6) +
 *            16*a(n-7) + 5*a(n-8) - 5*a(n-9) - 14*a(n-10) - 7*a(n-11) -
 *            6*a(n-12) + 3*a(n-14) + 8*a(n-15) + 6*a(n-16) + 4*a(n-17) for n
 *            > 19 with a(1)=4, a(2)=13, a(3)=20, a(4)=26, a(5)=43, a(6)=72,
 *            a(7)=115, a(8)=189, a(9)=318, a(10)=526, a(11)=874...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static const seq_t init_vals[19] = {
    (seq_t)4,   (seq_t)13,  (seq_t)20,  (seq_t)26,  (seq_t)43,
    (seq_t)72,  (seq_t)115, (seq_t)189, (seq_t)318, (seq_t)526,
    (seq_t)874, (seq_t)1473,(seq_t)2482,(seq_t)4185,(seq_t)7085,
    (seq_t)12032,(seq_t)20448,(seq_t)34806,(seq_t)59371
};

static void grow_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failure */
    cache = tmp;
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i < 19) {
            cache[i] = init_vals[i];
        } else {
            seq_t val = 3 * cache[i-1] - cache[i-2] + 2 * cache[i-3] - 11 * cache[i-4]
                        + 4 * cache[i-6] + 16 * cache[i-7] + 5 * cache[i-8]
                        - 5 * cache[i-9] - 14 * cache[i-10] - 7 * cache[i-11] - 6 * cache[i-12]
                        + 3 * cache[i-14] + 8 * cache[i-15] + 6 * cache[i-16] + 4 * cache[i-17];
            cache[i] = val;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    grow_cache(n);
    return cache[(size_t)(n-1)];
}
