/*
 * A333038  Numbers m such that sigma(m) <= sigma(m-1).
 * https://oeis.org/A333038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333038: numbers m with sigma(m) <= sigma(m-1). */
#include "../utils/sigma_of.h"

/* 
 * Return the n-th term of the sequence (n >= 1). 
 * If n == 0 the call is undefined in the OEIS offset;  
 * we return 0 for safety.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n = 0 */

    idx_t m = 5;                    /* first candidate (first term is 5) */
    seq_t prev_sigma = sigma_of(4); /* sigma(4) = 7, used for first comparison */

    for (idx_t count = 1; ; ++m) { /* count runs over the terms found */
        seq_t cur_sigma = sigma_of(m);
        if (cur_sigma <= prev_sigma) {  /* condition for a term */
            if (count == n) return m;   /* found the n-th term */
            ++count;
        }
        prev_sigma = cur_sigma;        /* update for next iteration */
    }
}
