/*
 * A276061  Sum of the asymmetry degrees of all compositions of n into parts
 *          congruent to 1 mod 3.
 * https://oeis.org/A276061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 112,
 *            u128 n <= 226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276061 – Sum of the asymmetry degrees of all compositions of n
   into parts ≡ 1 (mod 3).
   Recurrence (order 12) with initial values a(0)…a(11) taken from the
   sequence table:
      a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
      a(5)=2, a(6)=2, a(7)=4, a(8)=6, a(9)=10,
      a(10)=18, a(11)=28.
   For n ≥ 12:
          a(n) = a(n‑1) + a(n‑2) + a(n‑3)
               - a(n‑4) - a(n‑6) - a(n‑7) - a(n‑8)
               - 2·a(n‑9) + a(n‑10) + a(n‑12)
   This follows from the rational generating function
   g(z) = 2z⁵(1–z³)/((1+z)(1–z+z²)(1+z–z³)(1–z–z³)²). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 11 */
    static const seq_t base[12] = {
        0,0,0,0,0,      /* a(0) … a(4)   */
        2,2,4,6,10,     /* a(5) … a(9)   */
        18,28            /* a(10) … a(11) */
    };

    /* Static storage for computed terms */
    static seq_t *buf = NULL;
    static idx_t  buf_cap = 0;   /* allocated capacity (in elements) */
    static idx_t  cur_max = 0;   /* highest computed index (inclusive) */
    static int    initialized = 0; /* flag */

    /* Initialise the buffer on first call */
    if (!initialized) {
        buf_cap = 12;
        buf = (seq_t*)malloc(buf_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 12; ++i)
            buf[i] = base[i];
        cur_max = 11;                /* a[0] … a[11] are ready */
        initialized = 1;
    }

    /* Grow buffer if we need to reach index n */
    if (n + 1 > buf_cap) {
        idx_t new_cap = buf_cap ? buf_cap << 1 : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));

        /* Compute terms for the newly allocated slots */
        for (idx_t i = buf_cap; i < new_cap; ++i) {
            if (i < 12) {
                buf[i] = base[i];
            } else {
                seq_t sum_pos = buf[i-1] + buf[i-2] + buf[i-3]
                                + buf[i-10] + buf[i-12];
                seq_t sum_sub = buf[i-4] + buf[i-6] + buf[i-7]
                                + buf[i-8] + buf[i-9] + buf[i-9];
                buf[i] = sum_pos - sum_sub;
            }
        }

        buf_cap = new_cap;
        cur_max = buf_cap - 1;   /* all slots up to cur_max are now ready */
    }

    /* If we still don't have terms up to n, compute them */
    if (n > cur_max) {
        for (idx_t i = cur_max + 1; i <= n; ++i) {
            seq_t sum_pos = buf[i-1] + buf[i-2] + buf[i-3]
                            + buf[i-10] + buf[i-12];
            seq_t sum_sub = buf[i-4] + buf[i-6] + buf[i-7]
                            + buf[i-8] + buf[i-9] + buf[i-9];
            buf[i] = sum_pos - sum_sub;
        }
        cur_max = n;
    }

    return buf[n];
}
