/*
 * A301952  Number of 3Xn 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +12*a(n-2) -5*a(n-3) -14*a(n-4) +14*a(n-6)
 *            +4*a(n-7) -3*a(n-8) -5*a(n-9) +a(n-10) for n > 12 Initial values
 *            (empirical): a(1)=0, a(2)=5, a(3)=20, a(4)=123, a(5)=680,
 *            a(6)=4070, a(7)=23565, a(8)=138014, a(9)=805249, a(10)=4704141,
 *            a(11)=27469522, a(12)=160428571
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* NULL‑terminated sequence array (index 0 unused) */
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static int init_done = 0;

    /* ensure the cache can hold index n */
    if ((size_t)n + 1 > cap) {
        grow_seq((seq_t**)&cache, &cap, (size_t)n + 1);
        /* new slots are zero‑initialized by the alloca policy */
    }

    /* initial values */
    if (!init_done) {
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)5;
        cache[3]  = (seq_t)20;
        cache[4]  = (seq_t)123;
        cache[5]  = (seq_t)680;
        cache[6]  = (seq_t)4070;
        cache[7]  = (seq_t)23565;
        cache[8]  = (seq_t)138014;
        cache[9]  = (seq_t)805249;
        cache[10] = (seq_t)4704141;
        cache[11] = (seq_t)27469522;
        cache[12] = (seq_t)160428571;
        init_done = 1;
    }

    if (n <= 12) {          /* already known */
        return cache[(size_t)n];
    }

    /* compute missing entries up to n */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        seq_t pos = (seq_t)4 * cache[i - 1]  /* 4*a(i-1) */
                  + (seq_t)12 * cache[i - 2] /* 12*a(i-2) */
                  + (seq_t)14 * cache[i - 6] /* 14*a(i-6) */
                  + (seq_t)4  * cache[i - 7] /* 4*a(i-7) */
                  +        cache[i - 10];    /* +a(i-10) */

        seq_t neg = (seq_t)5  * cache[i - 3]  /* 5*a(i-3) */
                  + (seq_t)14 * cache[i - 4]  /* 14*a(i-4) */
                  + (seq_t)3  * cache[i - 8]  /* 3*a(i-8) */
                  + (seq_t)5  * cache[i - 9]; /* 5*a(i-9) */

        cache[i] = pos - neg;  /* pos > neg for all i > 12 */
    }

    return cache[(size_t)n];
}
