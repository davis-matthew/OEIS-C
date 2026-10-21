/*
 * A283104  Second differences of Stern's diatomic sequence (A002487).
 * https://oeis.org/A283104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s(n+2) - 2*s(n+1) + s(n) where s is Stern's diatomic
 *            sequence: s(0)=0, s(1)=1, for k>1, s(2k)=s(k),
 *            s(2k+1)=s(k)+s(k+1)
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Stern's sequence on demand and caches it. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *stern_seq = NULL;   /* cached Stern values */
static idx_t  stern_len = 0;      /* size of the allocated array */

/* Ensure stern_seq has entries up to index 'upto' (inclusive). */
static void ensure_stern(idx_t upto)
{
    /* If already sufficient, nothing to do. */
    if (upto + 1 <= stern_len)
        return;

    /* Grow the array: double until large enough. */
    idx_t old_len = stern_len;
    idx_t new_len = stern_len ? (stern_len << 1) : 8;
    while (new_len <= upto + 1)
        new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(stern_seq, (size_t)new_len * sizeof(seq_t));
    if (!tmp)
        abort();                   /* out of memory */

    stern_seq = tmp;
    stern_len = new_len;

    /* If this is the first allocation, set the base values. */
    if (old_len == 0) {
        stern_seq[0] = (seq_t)0;
        stern_seq[1] = (seq_t)1;
        old_len = 2;
    }

    /* Compute new entries using the recurrence. */
    for (idx_t i = old_len; i < new_len; ++i) {
        if ((i & 1) == 0) {               /* even */
            stern_seq[i] = stern_seq[i >> 1];
        } else {                          /* odd */
            stern_seq[i] = stern_seq[i >> 1] +
                            stern_seq[(i >> 1) + 1];
        }
    }
}

seq_t a(idx_t n)
{
    /* Need s(n), s(n+1), s(n+2). */
    ensure_stern(n + 2);

    seq_t sn  = stern_seq[(size_t)n];
    seq_t sn1 = stern_seq[(size_t)(n + 1)];
    seq_t sn2 = stern_seq[(size_t)(n + 2)];

    return sn2 - sn1 * (seq_t)2 + sn;      /* sn2 - 2*sn1 + sn */
}
