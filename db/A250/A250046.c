/*
 * A250046  Numbers n such that m = floor(n/7) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that m = floor(n/7) is coprime to
 *            n and, if m>0, m itself is a term of the sequence
 * Range:     u8 n <= 37, u16 n <= 2975, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated on demand and cached so that calls with arbitrary indices
   are answered in O(log k) time per candidate, where k is the number of terms
   already generated. */

/* Dependencies */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache of sequence terms */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    /* Initialisation: first term is 1 */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        /* Start searching from the next integer after the last produced term */
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;

        for (;;) {
            /* Compute m = floor(candidate/7) */
            seq_t m = candidate / (seq_t)7;

            /* Check if candidate satisfies the property */
            bool ok = false;

            if (m != 0) {
                /* Binary search to see whether m is already in the sequence */
                size_t lo = 0, hi = seq_len;
                while (lo < hi) {
                    size_t mid = lo + (hi - lo) / 2;
                    if (seq[mid] < m)
                        lo = mid + 1;
                    else
                        hi = mid;
                }
                if (lo < seq_len && seq[lo] == m) {
                    /* m is in the sequence; now test coprimality */
                    if (gcd(candidate, m) == (seq_t)1)
                        ok = true;
                }
            }

            if (ok) {
                /* Store candidate in the cache */
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = candidate;
                break;  /* go on to generate next term */
            } else {
                ++candidate;
            }
        }
    }

    /* Return the requested term */
    return seq[n - 1];
}
