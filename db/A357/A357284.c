/*
 * A357284  a(n) = (1/2)*A357283(n).
 * https://oeis.org/A357284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) - 6*a(n-3) + 10*a(n-4) - 4*a(n-5)
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static buffer to memoise computed values */
    static seq_t *buf  = NULL;   /* array of a(i) */
    static size_t cap   = 0;     /* allocated capacity (number of slots) */
    static idx_t  size  = 0;     /* number of terms actually stored (>=5 after init) */

    /* initialise with first five known values on the first call */
    if (!buf) {
        cap = 16;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;          /* out‑of‑memory: return 0 as a safe fallback */

        buf[0] = (seq_t)0;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)0;
        buf[3] = (seq_t)0;
        buf[4] = (seq_t)1;
        size = 5;
    }

    /* grow capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* realloc failed: return 0 */
        buf = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to index n (the recurrence is valid for n >= 5) */
    if (n + 1 > size) {                    /* need to compute new values */
        for (idx_t i = size; i <= n; ++i) {
            seq_t pos = ((seq_t)4 * buf[i-1]) + ((seq_t)10 * buf[i-4]);
            seq_t neg = ((seq_t)3 * buf[i-2]) + ((seq_t)6 * buf[i-3]) + ((seq_t)4 * buf[i-5]);
            buf[i] = pos - neg;            /* guaranteed non‑negative for n >= 5 */
        }
        size = n + 1;
    }

    return buf[n];
}
