/*
 * A296361  Number of monohedral disk tilings of type C^t_{3,n}.
 * https://oeis.org/A296361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 700, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A296361: Number of monohedral disk tilings of type C^t_{3,n}
   Recurrence (valid for n > 11):
       a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4)
              - 3*a(n-6) + 2*a(n-7) + 2*a(n-8) - 3*a(n-9) + a(n-10)
   Base values (for 1 ≤ n ≤ 11):
       a( 1)= 2 , a( 2)=62 , a( 3)=116 , a( 4)=200 , a( 5)=318 ,
       a( 6)=476 , a( 7)=682 , a( 8)=946 , a( 9)=1272 , a(10)=1674 , a(11)=2152
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Guard against the offset being out of bounds – not part of the OEIS spec */
    if (n == 0)
        return (seq_t)0;

    /* Static cache: buf[0] = a(1), buf[1] = a(2), … */
    static seq_t *buf          = NULL;   /* storage for computed terms */
    static idx_t capacity      = 0;      /* allocated size of buf               */
    static idx_t computed      = 0;      /* number of terms already computed    */
    static const seq_t base[11] = {
        2, 62, 116, 200, 318,
        476, 682, 946, 1272, 1674, 2152
    };

    /* Allocate or grow the buffer if needed */
    if (buf == NULL) {
        /* Ensure at least space for the base values */
        capacity = (n > 12 ? n : 12);
        buf = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!buf)
            abort();
    } else if (n > capacity) {
        capacity = n;
        seq_t *tmp = (seq_t *)realloc(buf, capacity * sizeof(seq_t));
        if (!tmp)
            abort();
        buf = tmp;
    }

    /* Compute missing terms up to index n-1 (0‑based) */
    for (idx_t i = computed; i < n; ++i) {
        if (i < 11) { /* base values */
            buf[i] = base[i];
        } else {
            /* Split the recurrence into positive and negative parts to avoid
               underflow on unsigned types.  sum_pos >= sum_neg for all valid n. */
            seq_t sum_pos = 3 * buf[i-1] + 3 * buf[i-4] + 2 * buf[i-7]
                            + 2 * buf[i-8] + buf[i-10];
            seq_t sum_neg = 2 * buf[i-2] + 2 * buf[i-3] + 3 * buf[i-6]
                            + 3 * buf[i-9];
            buf[i] = sum_pos - sum_neg;
        }
    }
    computed = n;  /* all terms up to n are now available */

    return buf[n-1];
}
