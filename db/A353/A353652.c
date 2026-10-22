/*
 * A353652  Unique monotonic sequence of positive integers satisfying a(a(n))
 *          = k*(n-1) + 3, where k = 5.
 * https://oeis.org/A353652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A353652 (k = 5)
   a(n) = n + 5^i            if  (5^i+1)/2 ≤ n < (3·5^i+1)/2,
   a(n) = 5·(n – 5^i) – 2    otherwise,
   where i = ⌊log_5(2·n – 1)⌋.
   The function works for any seq_t/idx_t pair (8,16,32,64,128 bits or big‑int).
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 (unused). */
    if (n == 0) return 0;

    /* 2·n – 1, using 128‑bit integer to avoid overflow when n is large. */
    __int128 v = (__int128)n * 2 - 1;

    /* Find i such that 5^i ≤ v < 5^(i+1). */
    __int128 p = 1;          /* current power 5^i */
    while (p * 5 <= v) {
        p *= 5;
    }

    /* Convert power to seq_t for final result. */
    seq_t power = (seq_t)p;

    /* Compute the boundary m_thresh = (3·5^i + 1) / 2.
       The calculation is made in 128‑bit first to avoid overflow. */
    __int128 m_tmp = p * 3 + 1;
    seq_t m_thresh = (seq_t)(m_tmp >> 1);   /* /2 */

    seq_t idx = (seq_t)n;

    if (idx < m_thresh) {
        /* First branch: a(n) = n + 5^i. */
        return idx + power;
    } else {
        /* Second branch: a(n) = 5·(n – 5^i) – 2. */
        seq_t diff = idx - power;
        return diff * 5 - 2;
    }
}
