/*
 * A283973  Numbers n such that A007306(n) = A283986(n); positions of zeros in
 *          A283988.
 * https://oeis.org/A283973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 4622, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n‑th positive integer i such that
   A002487(i-1) & A002487(i) == 0
   (indices where two successive Stern terms have no common 1‑bits).
   Computed by scanning Stern’s diatomic sequence and collecting
   qualifying indices. */
#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static arrays for the Stern sequence and qualifying indices   */
    static seq_t *stern   = NULL;
    static size_t stern_cap = 0;
    static size_t stern_len = 0;

    static seq_t *qual    = NULL;
    static size_t qual_cap = 0;
    static size_t qual_len = 0;

    if (n == 0)
        return (seq_t)0;  /* behaviour below offset is irrelevant */

    /* grow until we have at least n qualifying indices            */
    while (qual_len < n)
    {
        idx_t m = stern_len;          /* next Stern index to compute   */
        seq_t val;

        if (m == 0)
            val = (seq_t)0;
        else if (m == 1)
            val = (seq_t)1;
        else if ((m & 1ull) == 0)     /* even */
            val = stern[m / 2];
        else                          /* odd  */
            val = stern[(m - 1) / 2] + stern[(m + 1) / 2];

        /* ensure capacity for the Stern array  */
        if (stern_len + 1 > stern_cap)
            ensure_capacity(stern_len + 1, &stern_cap,
                            (void **)&stern, sizeof(seq_t));
        stern[stern_len] = val;

        /* check qualifier for m > 0  */
        if (m > 0) {
            seq_t prev = stern[stern_len - 1];
            if ((prev & val) == (seq_t)0) {
                if (qual_len + 1 > qual_cap)
                    ensure_capacity(qual_len + 1, &qual_cap,
                                    (void **)&qual, sizeof(seq_t));
                qual[qual_len++] = (seq_t)m;
            }
        }
        stern_len++;
    }
    return qual[n - 1];
}
