/*
 * A280227  Number of n X 2 0..1 arrays with no element unequal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n>7 with
 *            a(1)=0, a(2)=4, a(3)=6, a(4)=8, a(5)=14, a(6)=24, a(7)=42
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 42, u64 n <= 87, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static idx_t max_computed = 0;

/* Computes the n-th term of sequence A280227 (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behavior below offset is irrelevant */

    /* Grow cache if necessary */
    if (n > max_computed) {
        idx_t newlen = n;                  /* desired length of cache */
        /* realloc must be cast to the correct pointer type in C++ */
        seq_t *newc = (seq_t *)realloc(cache, (size_t)newlen * sizeof(seq_t));
        if (!newc) exit(1);                /* allocation failure */
        cache = newc;

        /* Compute new entries */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            switch ((int)i) {
                case 1: val = (seq_t)0;  break;
                case 2: val = (seq_t)4;  break;
                case 3: val = (seq_t)6;  break;
                case 4: val = (seq_t)8;  break;
                case 5: val = (seq_t)14; break;
                case 6: val = (seq_t)24; break;
                case 7: val = (seq_t)42; break;
                default:
                    val = (seq_t)2 * cache[i-2] + cache[i-3]
                          - (seq_t)2 * cache[i-4] - cache[i-5];
                    break;
            }
            cache[i-1] = val;
        }
        max_computed = n;
    }
    return cache[n-1];
}
