/*
 * A319028  Number of permutations pi of [n] such that s(pi) avoids the
 *          patterns 132 and 321, where s is West's stack-sorting map.
 * https://oeis.org/A319028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319028:
   Number of permutations π of [n] such that s(π) avoids patterns 132 and 321.
   Recurrence (n>3):
     n*(n+1)*a(n) = 4*n*(3*n-2)*a(n-1)
                    -4*(2*n-3)*(6*n-5)*a(n-2)
                    +16*(2*n-5)*(2*n-3)*a(n-3)                                   */

/* Standard integer header for uint64_t */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases (offset 1) */
    if (n == 0) return 0;          /* undefined for n<1; safe guard */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* Local variables holding the last three terms */
    seq_t p3 = (seq_t)1;   /* a(n-3) */
    seq_t p2 = (seq_t)2;   /* a(n-2) */
    seq_t p1 = (seq_t)6;   /* a(n-1) */

    /* Iterative computation of a(n) */
    for (idx_t i = 4; i <= n; ++i)
    {
#ifdef __cplusplus
        /* C++ branch: seq_t is an arbitrary‑precision integer type */
        seq_t coef1 = (seq_t)(4 * i * (3 * i - 2));
        seq_t coef2 = (seq_t)(4 * (2 * i - 3) * (6 * i - 5));
        seq_t coef3 = (seq_t)(16 * (2 * i - 5) * (2 * i - 3));

        seq_t num = p1 * coef1 - p2 * coef2 + p3 * coef3;
        seq_t denom = (seq_t)i * ((seq_t)(i + 1));
        seq_t cur = num / denom;
#else
        /* C branch: seq_t is an unsigned integer type (≤ 64‑bits).  Use 128‑bit
           intermediate arithmetic to avoid overflow. */
        unsigned __int128 coef1 = 4u * (unsigned __int128)i *
                                 ((unsigned __int128)3 * i - 2);
        unsigned __int128 coef2 = 4u * (unsigned __int128)(2 * i - 3) *
                                 ((unsigned __int128)6 * i - 5);
        unsigned __int128 coef3 = 16u * (unsigned __int128)(2 * i - 5) *
                                  (unsigned __int128)(2 * i - 3);

        unsigned __int128 num = (unsigned __int128)p1 * coef1
                               - (unsigned __int128)p2 * coef2
                               + (unsigned __int128)p3 * coef3;
        unsigned __int128 denom = (unsigned __int128)i * (i + 1);
        seq_t cur = (seq_t)(num / denom);
#endif
        /* Slide the window of last three terms */
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }

    return p1;
}
