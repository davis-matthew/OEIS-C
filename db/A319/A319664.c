/*
 * A319664  Irregular triangle read by rows: T(n,k) = (-3)^k mod 2^n, n >= 2,
 *          0 <= k <= 2^(n-2) - 1.
 * https://oeis.org/A319664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (-3)^k mod 2^r where r = floor(log2(n-1)) + 2 k = n -
 *            (2^(r-2) + 1) This implements the irregular triangle of A319664
 *            and returns the nth term (with offset 2)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Compute row r = floor(log2(n-1)) + 2.
       Count the number of bits in (n-1). */
    idx_t r = 0;
    idx_t t = n - 1;
    while (t) {
        t >>= 1;
        r++;
    }
    r += 1;                 /* r = floor(log2(n-1)) + 2 */

    /* Length of the row and starting index of this row. */
    uint64_t len   = ((uint64_t)1) << (r - 2);      /* 2^(r-2)   */
    uint64_t start = len + 1;                       /* 2^(r-2)+1 */
    uint64_t k = n - start;                         /* position in the row */

    /* Modulus 2^r and base (-3) mod modulus. */
    uint64_t mod  = ((uint64_t)1) << r;             /* 2^r        */
    uint64_t base = mod - 3;                       /* (-3) mod 2^r */

    /* Compute base^k mod mod via binary exponentiation. */
    uint64_t result = 1;
    uint64_t mask = mod - 1;                        /* low r bits mask */
    while (k) {
        if (k & 1)
            result = (result * base) & mask;
        base = (base * base) & mask;
        k >>= 1;
    }

    return (seq_t)result;
}
