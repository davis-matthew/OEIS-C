/*
 * A291626  Numbers k such that 1 is the smallest decimal digit of k^2.
 * https://oeis.org/A291626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the decimal digits of k^2
 *            contain no 0 and at least one 1. (Computed by a search.)
 * Range:     u8 n <= 7, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next_k = 1;   /* next candidate k to test */

    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        seq_t sq = (seq_t)next_k * (seq_t)next_k;
        bool no_zero = true;
        bool has_one  = false;
        seq_t tmp = sq;

        if (tmp == 0) {          /* only if k == 0, never happens here */
            no_zero = false;
        } else {
            while (tmp > 0) {
                seq_t d = tmp % 10;    /* last decimal digit of current residue     */
                if (d == 0) {          /* reject if any 0 digit is present          */
                    no_zero = false;
                    break;
                }
                if (d == 1) {
                    has_one = true;    /* mark presence of digit 1                 */
                }
                tmp /= 10;              /* shift decimal representation             */
            }
        }

        if (no_zero && has_one) {
            if (len == cap) {          /* enlarge buffer if necessary                 */
                cap <<= 1;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)next_k;
        }
        ++next_k;                       /* move to next candidate integer               */
    }

    return seq[n - 1];                   /* 1‑based indexing of the sequence              */
}
