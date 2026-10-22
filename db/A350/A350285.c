/*
 * A350285  a(n) is the least d > 0 such that A010060(n) = A010060(n - i*d)
 *          for i = 0..A350226(n)-1 (A010060 corresponds to the Thue-Morse
 *          sequence).
 * https://oeis.org/A350285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1279, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes sequence A350285.
 * For given n ≥ 0, find the maximum length L of an arithmetic progression
 * of equal Thue–Morse values ending at index n (i.e. with terms
 * n, n−d, n−2d, …, n−(L−1)d).  Then return the least positive common
 * difference d that achieves this maximum length.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n == 0: the sequence starts with 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Parity of the number of 1-bits at position n.  Thue-Morse value. */
    int tn = __builtin_parityll((unsigned long long)n);

    seq_t best_d = 1;
    int best_L = 1;

    /* Enumerate all possible common differences d from 1 to n */
    for (idx_t d = 1; d <= n; ++d) {
        idx_t pos = n;
        int parity = tn;
        int L = 1;          /* At least the term n itself */

        /* Extend backwards as far as the parity stays equal. */
        while (true) {
            if (pos < d) break;           /* would go before index 0 */
            idx_t next = pos - d;
            if (__builtin_parityll((unsigned long long)next) != parity) {
                break;                    /* parity mismatch */
            }
            ++L;
            pos = next;
        }

        /* Update best (maximum L, and minimal d for that L) */
        if (L > best_L) {
            best_L = L;
            best_d = (seq_t)d;
        } else if (L == best_L && d < best_d) {
            best_d = (seq_t)d;
        }
    }

    return best_d;
}
