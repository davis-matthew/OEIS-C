/*
 * A322939  a(n) = [x^n] (4*x^2 + x - 1)/(4*x^3 + 3*x^2 + 2*x - 1).
 * https://oeis.org/A322939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322939: a(n) = 4*a(n-3) + 3*a(n-2) + 2*a(n-1),  a(0)=a(1)=a(2)=1 */

#include <stdbool.h>

#define MAX_A_SIZE 20000   /* enough for n up to about 10000 */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_A_SIZE];
    static bool init = false;
    static idx_t last = 2;

    /* Initialise base terms once */
    if (!init) {
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        init = true;
    }

    if (n < 3)
        return (seq_t)1;

    /* Grow precomputed region up to n (if within bounds) */
    if (n >= MAX_A_SIZE) {
        /* For very large n we fall back to iterative recomputation each call.
           This case should not arise for the intended test range (n ≤ 10000). */
        seq_t a0 = 1, a1 = 1, a2 = 1, a3;
        idx_t i;
        for (i = 3; i <= n; ++i) {
            a3 = (seq_t)4 * a0 + (seq_t)3 * a1 + (seq_t)2 * a2;
            a0 = a1; a1 = a2; a2 = a3;
        }
        return a3;
    }

    /* Compute further terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-3] +
                   (seq_t)3 * cache[i-2] +
                   (seq_t)2 * cache[i-1];
    }
    last = (idx_t) (last < n ? n : last);

    return cache[n];
}
