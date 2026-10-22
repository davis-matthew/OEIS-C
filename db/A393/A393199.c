/*
 * A393199  a(n) is the least m such that the base m expansion of n contains
 *          exactly two ones.
 * https://oeis.org/A393199
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 9679, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least base m such that the base‑m representation of n
   contains exactly two digits equal to 1.  Sequence A393199 (offset 3). */
#include <stddef.h>
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;            /* the sequence is defined for n>=3 */
    for (idx_t m = 2; m <= n - 1; ++m) {   /* n-1 is always a valid base */
        idx_t tmp = n;
        int   cnt = 0;
        while (tmp) {
            if ((tmp % m) == 1) {
                ++cnt;
                if (cnt > 2) break;      /* early exit if more than 2 ones */
            }
            tmp /= m;
        }
        if (cnt == 2) return (seq_t)m;     /* minimal base found */
    }
    return (seq_t)(n - 1);                 /* fallback, should not happen */
}
