/*
 * A282105  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 427", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + 16*a(n-4) - 16*a(n-7)  for n ≥ 8
   Initial values: a(0)=1, a(1)=1, a(2)=7, a(3)=4, a(4)=29,
   a(5)=23, a(6)=124, a(7)=69 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored sequence values */
    static idx_t   cached = 0;    /* number of values already computed */
    static size_t  cap     = 0;   /* current allocation size */

    /* first time: allocate and initialise */
    if (!cache) {
        cap = 64;                 /* start with a small generous size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* safety, should never happen */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)29;
        cache[5] = (seq_t)23;
        cache[6] = (seq_t)124;
        cache[7] = (seq_t)69;
        cached   = 8;
    }

    /* if the requested term is already known, just return it */
    if (n < cached) return cache[n];

    /* otherwise compute further terms up to n */
    while (cached <= n) {
        /* grow the array if necessary */
        if (cached >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* safety */
            cache = tmp;
            cap   = newcap;
        }

        /* recurrence: a(k) = a(k-3) + 16*a(k-4) - 16*a(k-7)  */
        seq_t term1 = cache[cached - 3];
        seq_t term2 = cache[cached - 4] * (seq_t)16;
        seq_t term3 = cache[cached - 7] * (seq_t)16;
        seq_t value = term1 + term2 - term3;

        cache[cached++] = value;
    }

    return cache[n];
}
