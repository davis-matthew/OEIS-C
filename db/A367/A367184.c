/*
 * A367184  Starting index in the Period doubling sequence (A096268) of the
 *          first maximum length word in which every subword of length n is
 *          distinct.
 * https://oeis.org/A367184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a power of two; otherwise let p = 2^⌊log2 n⌋, k
 *            = n – p. If k < p/2 then a(n) = p + p/2 – k; else a(n) = p – k
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(n)) for nonzero n */
static idx_t log2_floor(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not useful for the sequence */
    idx_t m = log2_floor(n);              /* m = ⌊log2 n⌋ */
    idx_t p = ((idx_t)1) << m;            /* p = 2^m  */
    if (n == p) return 0;                 /* power‑of‑two case */
    idx_t k = n - p;                      /* offset from p  */
    if (k < (p >> 1))                     /* first branch  */
        return (seq_t)(p + (p >> 1) - k);
    else                                   /* second branch */
        return (seq_t)(p - k);
}
