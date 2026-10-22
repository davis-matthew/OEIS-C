/*
 * A338691  Positions of (-1)'s in A209615.
 * https://oeis.org/A338691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth term of { (2*k+1)*2^e : k+e is odd }
 * Range:     u8 n <= 128, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t last_scanned = 1;          /* last integer examined          */
    static seq_t *vals     = NULL;          /* array of found sequence values */
    static size_t cnt      = 0;             /* number of values stored        */
    static size_t cap      = 0;             /* current capacity of val array   */

    /* extend the table until we have at least n elements */
    while (cnt < n) {
        /* examine next integer */
        ++last_scanned;
        idx_t v = last_scanned;
        idx_t e = 0;

        /* factor out the highest power of two */
        while ((v & 1ULL) == 0) {
            v >>= 1;
            ++e;
        }

        /* v is now odd, write v = 2*k+1, so k = (v-1)/2 */
        idx_t k = (v - 1) / 2;

        /* include value if k+e is odd */
        if (((k + e) & 1ULL) == 1ULL) {
            if (cnt >= cap) {
                cap = (cap == 0) ? 16 : cap << 1;
                vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
            }
            vals[cnt++] = (seq_t)last_scanned;
        }
    }

    return vals[n - 1];
}
