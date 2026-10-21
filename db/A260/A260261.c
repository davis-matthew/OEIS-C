/*
 * A260261  Concatenation of their first digits yields the difference of two
 *          subsequent terms, a(1)=1. This is the lexicographically first
 *          infinite sequence with this property.
 * https://oeis.org/A260261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2438, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A260261 – Concatenation of first digits equals the gap.
   a(0)=0, a(1)=1.
   For n ≥ 2 let d₁ = firstdigit(a(n‑1)), d₂ = firstdigit(a(n)).
   Then a(n) = a(n‑1) + 10·d₁ + d₂, and we choose the smallest
   a(n) that satisfies this property.  The function memoises the
   sequence so that calls may appear in any order. */

#include <stdlib.h>
#include <stdint.h>

/* Compute the first decimal digit of x. */
static inline int first_digit(seq_t x)
{
    seq_t ten = (seq_t)10;
    while (x >= ten)
        x /= ten;
    return (int)x;
}

/* Memoised sequence. */
static seq_t *seq   = NULL;   /* array of computed terms      */
static idx_t  size  = 0;      /* number of allocated entries  */

seq_t a(idx_t n)
{
    /* Ensure the array holds at least two entries (0 and 1). */
    if (size < 2) {
        size  = 2;
        seq   = (seq_t *)realloc(seq, size * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
    }

    /* If the requested term is already known, return it. */
    if (n < size)
        return seq[n];

    /* Grow the array and compute missing terms up to n. */
    idx_t new_size = n + 1;
    seq = (seq_t *)realloc(seq, new_size * sizeof(seq_t));
    if (!seq) abort();

    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            seq[i] = (seq_t)0;
            continue;
        }
        if (i == 1) {
            seq[i] = (seq_t)1;
            continue;
        }

        seq_t    prev   = seq[i - 1];
        int      d1     = first_digit(prev);
        seq_t    cand   = 0;
        int      d2;

        for (d2 = 0; d2 <= 9; ++d2) {
            cand = prev + (seq_t)(10 * d1) + (seq_t)d2;
            if (first_digit(cand) == d2) {
                seq[i] = cand;
                break;
            }
        }
    }

    size = new_size;
    return seq[n];
}
