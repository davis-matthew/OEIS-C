/*
 * A290808  Number of partitions of n into distinct Pell parts (A000129).
 * https://oeis.org/A290808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n can be written as a sum of distinct Pell numbers
 *            (1,2,5,12,29, …); otherwise a(n) = 0. This is implemented by a
 *            static subset–sum DP that is recomputed only when a larger n is
 *            requested, so calls may occur in any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, free */
#include <stdint.h>   /* uint8_t */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Static memoization state */
    static idx_t   cached_max   = 0;      /* largest n that has been processed */
    static uint8_t *reach      = NULL;   /* reach[s] == 1 iff s is reachable */
    static size_t   reach_cap   = 0;      /* capacity of reach array */

    /* Initialisation for the very first call */
    if (reach == NULL)
    {
        reach_cap = 1;
        reach = (uint8_t *)calloc(reach_cap, sizeof(uint8_t));
        if (reach == NULL) return (seq_t)0;   /* allocation failure */
        reach[0] = 1;                         /* sum 0 is always reachable */
    }

    /* Rebuild DP if n is larger than any previously processed value */
    if ((size_t)n > cached_max)
    {
        idx_t new_max = n;

        /* Generate Pell numbers up to new_max (they are far fewer than new_max) */
        idx_t pell[32];
        size_t pcount = 0;

        if (new_max >= 1) {
            pell[pcount++] = 1;                  /* P1 = 1 */
        }
        if (new_max >= 2) {
            pell[pcount++] = 2;                  /* P2 = 2 */
            idx_t a = 1, b = 2;
            while (1) {
                idx_t c = 2 * b + a;             /* Pk = 2*Pk-1 + Pk-2 */
                if (c > new_max) break;
                if (pcount >= sizeof(pell)/sizeof(pell[0])) break;
                pell[pcount++] = c;
                a = b;
                b = c;
            }
        }

        /* Allocate new reach array with size new_max + 1 */
        size_t new_cap = (size_t)new_max + 1;
        uint8_t *newreach = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
        if (newreach == NULL) return (seq_t)0;  /* allocation failure */

        newreach[0] = 1; /* sum 0 reachable */

        /* Subset–sum DP over distinct Pell parts */
        for (size_t i = 0; i < pcount; ++i) {
            idx_t p = pell[i];
            for (idx_t s = new_max; s >= p; --s) {
                if (newreach[s - p]) newreach[s] = 1;
            }
        }

        /* Replace the old array */
        free(reach);
        reach      = newreach;
        reach_cap  = new_cap;
        cached_max = new_max;
    }

    /* n is guaranteed to be ≤ cached_max now */
    return (seq_t)(reach[(size_t)n] ? 1 : 0);
}
