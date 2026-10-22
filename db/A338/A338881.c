/*
 * A338881  Product of the nonzero digits of (n written in base 8).
 * https://oeis.org/A338881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the nonzero digits of n written in base 8,
 *            with a(0) = 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;          /* start with multiplicative identity      */
    while (n > 0)                  /* iterate over base‑8 digits from least   */
    {                              /* significant to most significant digit   */
        uint64_t d = n % 8;        /* idx_t is an unsigned 64‑bit integer     */
        if (d != 0)
            prod *= (seq_t)d;      /* multiply by non‑zero digit              */
        n /= 8;
    }
    return prod;                   /* for n==0 the loop is skipped → prod==1  */
}
