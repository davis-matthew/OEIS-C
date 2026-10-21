/*
 * A261933  The first of two consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of seventeen
 *          consecutive positive integers.
 * https://oeis.org/A261933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261933: generate by Pell-type recurrence.
   The first of two consecutive positive integers whose squares sum equals
   the sum of squares of seventeen consecutive positive integers.
   The sequence is obtained from the diophantine equation
   X^2 - 34 Y^2 = 815, where X = 2*m+1 and Y = n+8.
   Initial solutions (X,Y) = (81,13) and (183,31).
   Recurrence: (X',Y') = (35*X + 204*Y, 6*X + 35*Y).
   The sequence of m are (X-1)/2 from each solution in increasing order.
*/

#include <stdlib.h>

/* seq_t and idx_t types are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Seed solutions of the Pell equation X^2 - 34 Y^2 = 815. */
    struct Seed { seq_t X, Y; };
    static struct Seed seed[2] = { { (seq_t)81, (seq_t)13 },
                                   { (seq_t)183, (seq_t)31 } };

    /* Handle requested index. */
    if (n == 0) return 0;  /* undefined for offset 1; defensive return */

    /* Make space for request. */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < n) new_cap <<= 1;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms up to n. */
        while (cache_len < (size_t)n) {
            seq_t m0 = (seed[0].X - (seq_t)1) / (seq_t)2;
            seq_t m1 = (seed[1].X - (seq_t)1) / (seq_t)2;

            int use0;
            if (m0 < m1) use0 = 1;
            else use0 = 0;

            cache[cache_len++] = use0 ? m0 : m1;

            /* Advance the chosen seed using the recurrence. */
            struct Seed *s = use0 ? &seed[0] : &seed[1];
            seq_t X = s->X;
            seq_t Y = s->Y;
            seq_t newX = X * (seq_t)35 + Y * (seq_t)204;  /* 6*34 = 204 */
            seq_t newY = X * (seq_t)6  + Y * (seq_t)35;
            s->X = newX;
            s->Y = newY;
        }
    }
    return cache[(size_t)n - 1];
}
