/*
 * A315738  Coordination sequence Gal.6.343.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10); Initial values: a(0)=1, a(1)=6, a(2)=12, a(3)=18,
 *            a(4)=23, a(5)=27, a(6)=32, a(7)=37, a(8)=41, a(9)=46, a(10)=52
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* small pre‑computed values for n<=10 */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)6,  (seq_t)12, (seq_t)18, (seq_t)23,
        (seq_t)27, (seq_t)32, (seq_t)37, (seq_t)41, (seq_t)46,
        (seq_t)52
    };
    if (n <= 10) return init[(size_t)n];

    /* Memoisation buffer */
    static seq_t *buf = NULL;
    static size_t cap = 0;          /* capacity of buf                 */
    static idx_t  last = (idx_t)-1; /* highest index that has been   */
                                   /* computed and stored in buf      */

    /* initialise buffer on first use */
    if (buf == NULL) {
        cap = (size_t)n + 1;
        if (cap < 11) cap = 11; /* at least space for the first 11 terms */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) abort();     /* out of memory; terminate          */
        for (size_t i = 0; i < 11; ++i) buf[i] = init[i];
        last = 10;
    }

    /* grow buffer if needed */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        cap = required;
        seq_t *tmp = (seq_t *)realloc(buf, cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        buf = tmp;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum1 = buf[i-1] + buf[i-3] + buf[i-7] + buf[i-9];
        seq_t sum2 = buf[i-4] + buf[i-6] + buf[i-10];
        buf[i] = sum1 - sum2;   /* guaranteed non‑negative */
    }
    last = n;

    return buf[(size_t)n];
}
