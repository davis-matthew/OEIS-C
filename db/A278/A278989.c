/*
 * A278989  a(n) is the number of words of length n over an alphabet of size 4
 *          that are in standard order and which have the property that every
 *          letter that appears in the word is repeated.
 * https://oeis.org/A278989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278989
   a(n) defined by the linear recurrence with order 10:
     a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=4, a(5)=11, a(6)=41,
     a(7)=162, a(8)=715, a(9)=3425, a(10)=16777, a(11)=80928,
     a(12)=379347, a(13)=1726375, a(14)=7654817
   For n>14:
     a(n)=20*a(n-1)-175*a(n-2)+882*a(n-3)-2835*a(n-4)+6072*a(n-5)
          -8777*a(n-6)+8458*a(n-7)-5204*a(n-8)+1848*a(n-9)-288*a(n-10)
   This implementation memoises the values in a static array so that
   calls may be made in any order.
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static const seq_t init_terms[15] = {
    (seq_t)0,  (seq_t)0,  (seq_t)1,  (seq_t)1,  (seq_t)4,
    (seq_t)11, (seq_t)41, (seq_t)162, (seq_t)715, (seq_t)3425,
    (seq_t)16777, (seq_t)80928, (seq_t)379347,
    (seq_t)1726375, (seq_t)7654817
};

seq_t a(idx_t n)
{
    if (n < 15)
        return init_terms[n];

    if (cache == NULL) {
        cache = (seq_t *)malloc(15 * sizeof(seq_t));
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init_terms[i];
        cache_size = 15;
    }

    if ((idx_t)n >= (idx_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)20 * cache[i - 1]
                        - (seq_t)175 * cache[i - 2]
                        + (seq_t)882 * cache[i - 3]
                        - (seq_t)2835 * cache[i - 4]
                        + (seq_t)6072 * cache[i - 5]
                        - (seq_t)8777 * cache[i - 6]
                        + (seq_t)8458 * cache[i - 7]
                        - (seq_t)5204 * cache[i - 8]
                        + (seq_t)1848 * cache[i - 9]
                        - (seq_t)288 * cache[i -10];
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
