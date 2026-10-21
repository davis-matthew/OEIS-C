/*
 * A248056  Positions of 0,0 in the Thue-Morse sequence (A010060).
 * https://oeis.org/A248056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th occurrence of two consecutive 0s in
 *            the Thue-Morse sequence
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Thue-Morse bit at position i (1‑based) is parity(popcount(i-1)).            */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* undefined for n==0, but return 0 to avoid UB */
    }

    idx_t i = 1;                     /* position in the Thue‑Morse sequence (1‑based) */
    idx_t count = 0;                 /* number of 0‑0 pairs found so far */

    while (true) {
        /* Parity of popcount gives the Thue‑Morse bit */
        seq_t t_i   = (__builtin_popcountll(i - 1) & 1LL) == 0;
        seq_t t_ip1 = (__builtin_popcountll(i)     & 1LL) == 0;

        if (t_i && t_ip1) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
        ++i;
    }
}
