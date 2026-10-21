/*
 * A282262  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 446", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-2) + a(n-8) - 100*a(n-10) for n > 17 Initial
 *            values (OEIS A282262): a(0)=1, a(1)=11, a(2)=11, a(3)=1111,
 *            a(4)=11, a(5)=111, a(6)=101011, a(7)=11111111, a(8)=10000011,
 *            a(9)=1100000111, a(10)=1010101011, a(11)=111111111111,
 *            a(12)=100100000011, a(13)=11001010000111, a(14)=100111001010...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;  /* first index that has been computed */
    static bool initialized = false;

    /* Make sure the cache is large enough */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 32;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Initialise first 21 terms once */
    if (!initialized) {
        seq_t init[21] = {
            (seq_t)1ULL,          /* 0 */
            (seq_t)11ULL,         /* 1 */
            (seq_t)11ULL,         /* 2 */
            (seq_t)1111ULL,       /* 3 */
            (seq_t)11ULL,         /* 4 */
            (seq_t)111ULL,        /* 5 */
            (seq_t)101011ULL,     /* 6 */
            (seq_t)11111111ULL,   /* 7 */
            (seq_t)10000011ULL,   /* 8 */
            (seq_t)1100000111ULL, /* 9 */
            (seq_t)1010101011ULL, /*10*/
            (seq_t)111111111111ULL, /*11*/
            (seq_t)100100000011ULL, /*12*/
            (seq_t)11001010000111ULL, /*13*/
            (seq_t)10011100101011ULL, /*14*/
            (seq_t)1100101001111111ULL, /*15*/
            (seq_t)1001110010000011ULL, /*16*/
            (seq_t)110011100100000111ULL, /*17*/
            (seq_t)100111001010101011ULL, /*18*/
            (seq_t)11001110011111111111ULL, /*19*/
            (seq_t)10011100100100000011ULL /*20*/
        };
        for (idx_t i = 0; i < 21; ++i) cache[i] = init[i];
        max_computed = 21;
        initialized = true;
    }

    if (n < (idx_t)max_computed) return cache[n];

    /* Compute values from max_computed up to n */
    for (idx_t i = max_computed; i <= n; ++i) {
        seq_t val = cache[i - 2] * 100;
        val += cache[i - 8];
        val -= cache[i - 10] * 100;
        cache[i] = val;
    }

    max_computed = n + 1;
    return cache[n];
}
