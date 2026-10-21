/*
 * A271804  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 401", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=4, a(2)=32, a(3)=164, a(4)=744, a(5)=3196
   a(n) = 10*a(n-1) -33*a(n-2) +36*a(n-3) +4*a(n-4) -16*a(n-5)  for n>=6 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;
    if ((size_t)n + 1 > cached) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure – unlikely in tests */
        cache = tmp;
        /* Base values */
        static const seq_t base[6] = {
            (seq_t)1,
            (seq_t)4,
            (seq_t)32,
            (seq_t)164,
            (seq_t)744,
            (seq_t)3196
        };
        for (size_t i = cached; i < new_size; ++i) {
            if (i <= 5) {
                cache[i] = base[i];
            } else {
                unsigned __int128 val =
                    (unsigned __int128)10 * (unsigned __int128)cache[i-1];
                val -= (unsigned __int128)33 * (unsigned __int128)cache[i-2];
                val += (unsigned __int128)36 * (unsigned __int128)cache[i-3];
                val += (unsigned __int128)4  * (unsigned __int128)cache[i-4];
                val -= (unsigned __int128)16 * (unsigned __int128)cache[i-5];
                cache[i] = (seq_t)val;
            }
        }
        cached = new_size;
    }
    return cache[n];
}
