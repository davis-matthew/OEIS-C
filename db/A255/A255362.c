/*
 * A255362  Numbers n such that neither n nor n+1 is representable as x*y+x+y,
 *          where x>=y>1.
 * https://oeis.org/A255362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255362
 * a(n) = the n‑th nonnegative integer such that neither n nor n+1 equals
 *        x*y + x + y for integers x ≥ y > 1.  The value is found by
 *        testing representability of candidate numbers and memoising the
 *        sequence between calls.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Check whether k is representable as x*y + x + y with x ≥ y > 1.
 * The test is equivalent to checking whether k+1 has a divisor d
 * with 3 ≤ d ≤ sqrt(k+1).  For k+1 < 9 no such divisor exists. */
static bool is_representable(seq_t k)
{
    seq_t m = k + (seq_t)1;            /* m = k + 1 */
    if (m < (seq_t)9)                 /* smallest representable is 8 */
        return false;

    /* loop over possible divisors starting from 3 */
    for (idx_t d = 3; ; ++d) {
        seq_t dseq = (seq_t)d;
        seq_t d2   = dseq * dseq;
        if (d2 > m)                 /* reached sqrt(m) */
            break;
        if (m % dseq == (seq_t)0)
            return true;            /* found divisor ≥ 3 with co‑factor ≥ 3 */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* memoisation storage */
    static seq_t *seq      = NULL;    /* array of terms */
    static size_t seq_cap  = 0;       /* allocated capacity */
    static size_t seq_len  = 0;       /* number of terms stored */

    if (n == 0)
        return (seq_t)0;              /* undefined for offset 0, harmless */

    /* ensure we have enough terms precomputed */
    if (seq_len < n) {
        seq_t cand = (seq_len > 0) ? seq[seq_len - 1] + (seq_t)1 : (seq_t)0;

        while (seq_len < n) {
            /* test candidate cand and cand+1 */
            if (!is_representable(cand) && !is_representable(cand + (seq_t)1)) {
                /* grow array if necessary */
                if (seq_len == seq_cap) {
                    size_t newcap = seq_cap ? seq_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
                    if (!tmp)
                        return (seq_t)0;   /* allocation failure */

                    /* copy existing elements */
                    for (size_t i = 0; i < seq_len; ++i)
                        tmp[i] = seq[i];

                    /* free old block and adopt new one */
                    free(seq);
                    seq = tmp;
                    seq_cap = newcap;
                }
                seq[seq_len++] = cand;
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
