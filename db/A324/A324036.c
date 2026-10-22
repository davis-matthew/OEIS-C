/*
 * A324036  Modified reduced Collatz map fs acting on positive odd integers.
 * https://oeis.org/A324036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = fs(1 + 2*n), where * n mod 4 == 0 --> (2 + 3*n)/2 * n
 *            mod 4 == 1 --> 2 + 3*n * n mod 4 == 2 --> n/2 * n mod 4 == 3 -->
 *            2 + 3*n * This is the closed‑form expression valid for all n >=
 *            0
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine n modulo 4.  Using bitwise AND avoids any modulo
       computation and works for unsigned indices. */
    idx_t mod = n & 3ULL;

    switch (mod) {
    case 0:
        /* (2 + 3*n)/2 */
        return ((seq_t)2 + (seq_t)3 * (seq_t)n) / (seq_t)2;
    case 1:
    case 3:
        /* 2 + 3*n */
        return (seq_t)2 + (seq_t)3 * (seq_t)n;
    case 2:
        /* n/2 */
        return (seq_t)n / (seq_t)2;
    }
    /* The switch covers all cases of mod 0..3; return 0 as fallback. */
    return (seq_t)0;
}
