/*
 * A240916  a(n) = 6*a(n-1) + 2*2^(n-1) - 2 for n > 2, a(0) = a(1) = 0, a(2) =
 *          3.
 * https://oeis.org/A240916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=3; for n>2: a(n)=6*a(n-1)+2*2^(n-1)-2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* stored values of the sequence */
static idx_t  computed_up_to = 0;     /* greatest index already computed   */
static seq_t  pow2_last = (seq_t)0;   /* 2^(computed_up_to-1) (valid only if computed_up_to>=2) */

seq_t a(idx_t n)
{
    if (n < 2)                 /* a(0) and a(1) */
        return (seq_t)0;

    /* first computation request (n >= 2) – initialise base values */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);   /* indices 0,1,2 */
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        computed_up_to = 2;
        pow2_last = (seq_t)2;                   /* 2^(2-1) */
    }

    /* extend cache and fill missing terms up to n */
    if (n > computed_up_to) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            pow2_last = pow2_last * (seq_t)2;                           /* 2^(i-1) */
            cache[i] = cache[i-1] * (seq_t)6
                     + ((seq_t)2 * pow2_last)
                     - (seq_t)2;
        }
        computed_up_to = n;
    }

    return cache[n];
}
