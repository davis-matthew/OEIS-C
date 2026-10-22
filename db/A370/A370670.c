/*
 * A370670  Expansion of Sum_{k>=0} k! * ( x/(1+x^3) )^k.
 * https://oeis.org/A370670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + a(n-3) + (n-6)*a(n-4) + 2*a(n-6) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=23, a(5)=116,
 *            a(6)=702
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc and free */

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)6;
        case 4: return (seq_t)23;
        case 5: return (seq_t)116;
        case 6: return (seq_t)702;
        default: break;
    }

    /* Allocate a temporary buffer to hold the sequence up to n.
       The buffer is sized n+1 because we store a[0]..a[n]. */
    seq_t *buf = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!buf) return (seq_t)0;   /* graceful failure if memory cannot be allocated */

    /* Initialize the known first terms */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)2;
    buf[3] = (seq_t)6;
    buf[4] = (seq_t)23;
    buf[5] = (seq_t)116;
    buf[6] = (seq_t)702;

    /* Compute terms from i = 7 up to n using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        buf[i] = (seq_t)i * buf[i - 1]
               + buf[i - 3]
               + (seq_t)(i - 6) * buf[i - 4]
               + (seq_t)2 * buf[i - 6];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
