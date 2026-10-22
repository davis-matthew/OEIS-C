/*
 * A387373  Number of connected dominating sets in the n-Cameron graph.
 * https://oeis.org/A387373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 53*a(n-1)+209*a(n-2)+376*a(n-3)-673*a(n-4)+71*a(n-5) *
 *            -4479*a(n-6)+2120*a(n-7)-428*a(n-8)+320*a(n-9)+16*a(n-10); *
 *            Initial values: a(0)=5, a(1)=281, a(2)=15949, a(3)=905469, *
 *            a(4)=51427345, a(5)=2920698969, a(6)=165875085993, *
 *            a(7)=9420532825569, a(8)=535019698383817, a(9)=3038533840625...
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache storing computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initial values for n = 0 .. 9 */
    static const seq_t init[10] = {
        (seq_t)5ULL, (seq_t)281ULL, (seq_t)15949ULL,
        (seq_t)905469ULL, (seq_t)51427345ULL,
        (seq_t)2920698969ULL, (seq_t)165875085993ULL,
        (seq_t)9420532825569ULL,
        (seq_t)535019698383817ULL, (seq_t)30385338406252141ULL
    };

    /* Ensure cache can hold at least n+1 entries */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        /* Allocate or grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {          /* allocation failure */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialize newly needed entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                /* Compute a(i) by the recurrence, avoiding signed intermediates */
                seq_t pos = 0;
                pos += (seq_t)53   * cache[i-1];
                pos += (seq_t)209  * cache[i-2];
                pos += (seq_t)376  * cache[i-3];
                pos += (seq_t)71   * cache[i-5];
                pos += (seq_t)2120 * cache[i-7];
                pos += (seq_t)320  * cache[i-9];
                pos += (seq_t)16   * cache[i-10];

                seq_t neg = 0;
                neg += (seq_t)673  * cache[i-4];
                neg += (seq_t)4479 * cache[i-6];
                neg += (seq_t)428  * cache[i-8];

                cache[i] = pos - neg;   /* pos >= neg by definition */
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
