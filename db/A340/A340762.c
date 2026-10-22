/*
 * A340762  Numbers k such that iterations of phi(k), phi(phi(k)), ... end in
 *          ... 4, 2, 1.
 * https://oeis.org/A340762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that repeated applications of Euler’s totient φ
 *            eventually reach 4 (then 2, then 1). The sequence is indexed
 *            from a(1)=4
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/calc_phi.h"


seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* table of known elements          */
    static size_t len = 0;         /* number of elements stored        */
    static size_t cap = 0;         /* current capacity of table        */
    static seq_t last_checked = 0; /* largest integer already tested   */

    /* n < 1 is outside the offset; return 0 (unused) */
    if (n == 0) return 0;

    /* initialise storage and the first element if this is the first call */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        len = 0;
        last_checked = 4;          /* start with the first term  */
        seq[len++] = 4;            /* a(1) = 4                     */
    }

    /* extend the sequence until at least n terms are available */
    while (len < n) {
        ++last_checked;            /* next candidate */

        seq_t t = last_checked;
        /* iterate φ until 4 or 1 is reached */
        while (t != 4 && t != 1) {
            t = calc_phi(t);
        }
        if (t == 4) {              /* accept the candidate */
            if (len == cap) {      /* grow the table if needed */
                cap <<= 1;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = last_checked;
        }
    }

    return seq[n - 1];
}
