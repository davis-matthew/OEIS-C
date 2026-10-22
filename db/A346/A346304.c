/*
 * A346304  Positions of words in A076478 that start with 1 and end with 0.
 * https://oeis.org/A346304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A346304: Positions of words in A076478 that start with 1 and end with 0.
 * Formula derived:
 *   Let m = n + 1.
 *   Let L = ceil(log2(m)) + 1.
 *   Then a(n) = 2^L + 2*n - 1.
 * Where 2^L is computed in the type seq_t.
 */
#include <stdint.h>

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* offset starts at 1, but compute as described for n >= 1 */
    idx_t m = n + 1;                 /* m = n + 1 > 0 */

    /* floor_log2(m) using builtin if available, else iterative */
    int floor_log;
#if defined(__GNUC__) || defined(__clang__)
    /* __builtin_clzll is defined for unsigned long long */
    floor_log = 63 - __builtin_clzll((unsigned long long)m);
#else
    floor_log = 0;
    idx_t t = m;
    while (t > 1) {
        t >>= 1;
        floor_log++;
    }
#endif

    /* determine L: ceil(log2(m)) + 1 */
    int L;
    if ((m & (m - 1)) == 0) {            /* m is a power of two */
        L = floor_log + 1;
    } else {
        L = floor_log + 2;
    }

    /* compute pow2 = 2^L in seq_t (avoid undefined shift on seq_t) */
    seq_t pow2 = 1;
    for (int i = 0; i < L; ++i) {
        pow2 <<= 1;
    }

    /* a(n) = pow2 + 2*n - 1 */
    seq_t result = pow2 + ((seq_t)2 * (seq_t)n) - 1;
    return result;
}
