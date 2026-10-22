/*
 * A309363  Van Eck's sequence (cf. A181391), but outputting 2 for a new
 *          number, not 0.
 * https://oeis.org/A309363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0. For n > 1, a(n) = a(n-1) if a(n-1) has not appeared
 *            earlier, otherwise a(n) = (n-1) - (index of previous occurrence
 *            of a(n-1)). That is, a(n) = k where k > 0 and a(n-k) = a(n-1),
 *            or 2 if no such k exists. This implements Van Eck's sequence
 *            with 2 marking new values
 * Range:     u8 n <= 412, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The build supplies the integer types seq_t and idx_t. */
static seq_t *seq        = NULL;   /* array holding the sequence values          */
static size_t seq_len    = 0;       /* number of terms currently stored           */
static size_t seq_cap    = 0;       /* current capacity of 'seq'                  */

static void ensure_capacity(size_t needed)
{
    /* Allocate a fixed buffer large enough for the expected maximum n.
       Tests only require up to about 10 000, but allocate a bit more
       to be safe and avoid multiple reallocations. */
    if (needed <= seq_cap)
        return;

    size_t newcap = needed + 100;          /* extra slack */
    if (newcap < 16)
        newcap = 16;

    /* realloc produces a void* which is then cast to seq_t*. */
    seq_t *newptr = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq = newptr;
    seq_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined: return 0 for safety. */
        return (seq_t)0;
    }

    /* Initialise the sequence the first time a() is called. */
    if (seq == NULL) {
        ensure_capacity(20000);                /* safety margin for n ≤ 10 000 */
        seq[0] = (seq_t)0;                     /* a(1) = 0 */
        seq_len = 1;
    }

    /* Generate terms up to 'n'. */
    while (seq_len < (size_t)n) {
        seq_t curr = seq[seq_len - 1];         /* a(seq_len) (current term) */

        /* Search backward for the previous occurrence of 'curr'. */
        size_t prev_index = SIZE_MAX;          /* sentinel: not found */
        if (seq_len >= 2) {
            for (size_t i = seq_len - 1; i-- > 0; ) {
                if (seq[i] == curr) {
                    prev_index = i;
                    break;
                }
            }
        }

        seq_t next;
        if (prev_index != SIZE_MAX) {
            /* prev_index is 0‑based, seq_len is 1‑based index of curr. */
            next = (seq_t)((seq_len - 1) - prev_index);
        } else {
            next = (seq_t)2;                   /* new value case */
        }

        /* Append the next term. */
        seq_len++;
        ensure_capacity(seq_len);
        seq[seq_len - 1] = next;
    }

    return seq[n - 1];
}
