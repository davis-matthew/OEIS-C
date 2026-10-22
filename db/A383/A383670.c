/*
 * A383670  Limiting word, as a sequence, obtained by prefixing with 0 the
 *          limiting sequence of s(n) defined by s(0) = 0, s(1) = 12, s(n) =
 *          the concatenation of s(n - 1) and s(n - 2).
 * https://oeis.org/A383670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2, a(n) is the (n-2)-th digit of the infinite
 *            Fibonacci word constructed with seeds s(1)="12", s(2)="120" and
 *            rule s(k)=s(k-1)‖s(k-2). The leading 0 is the first term in the
 *            sequence
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle the prefixed 0 */
    if (n <= 1u) return (seq_t)0;

    /* Index within the Fibonacci word (0‑based) */
    idx_t k = n - 2u;

    /* Build Fibonacci‑word lengths: len[1]=2 (S1="12"), len[2]=3 (S2="120") */
    seq_t len[100];            /* enough for all practical N (<10^4) */
    size_t size = 2;
    len[1] = (seq_t)2;
    len[2] = (seq_t)3;
    while (len[size] <= (seq_t)k) {
        len[++size] = len[size] + len[size - 1];
    }
    size_t m = size;          /* index of the shortest word covering k */

    /* Iteratively descend the recursion tree */
    while (m > 2u) {
        if (k < (idx_t)len[m - 1]) {
            m = m - 1u;
        } else {
            k -= (idx_t)len[m - 1];
            m = m - 2u;
        }
    }

    /* Base cases */
    if (m == 1u) {                    /* S1 = "12" */
        return (k == 0u) ? (seq_t)1 : (seq_t)2;
    } else {                           /* m == 2, S2 = "120" */
        if (k == 0u) return (seq_t)1;
        if (k == 1u) return (seq_t)2;
        return (seq_t)0;
    }
}
