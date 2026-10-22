/*
 * A346040  a(n) is 1w' converted to decimal, where the binary word w' is the
 *          result of applying Post's tag system {00,1101} to the binary word
 *          w, where 1w is n converted to binary (the leftmost 1 acts as a
 *          delimiter).
 * https://oeis.org/A346040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = delete(append(n)) with append(1) = 1; append(n>1) =
 *            2^(2+2s) * n + 13s where s = floor((n-2^k)/2^(k-1)) delete(n) =
 *            n - 2^t * (floor(n/2^t)-1), t = max(log2(n)-3,0)
 * Range:     u8 n <= 23, u16 n <= 7163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* compute append(n) according to the given formula */
static inline seq_t append_seq(idx_t n)
{
    if (n == 1u)
        return (seq_t)1u;

    idx_t k = 63u - __builtin_clzll(n);                    /* floor(log2(n)) */
    idx_t s = (n - ((idx_t)1 << k)) >> (k - 1u);           /* second highest bit */

    seq_t pow2 = ((seq_t)1 << (2u + 2u * s));               /* 2^(2+2s) */
    seq_t res   = pow2 * (seq_t)n + (seq_t)13 * (seq_t)s;
    return res;
}

/* compute delete(n) according to the given formula */
static inline seq_t delete_seq(seq_t n)
{
    unsigned k = bitlen_seq_t(n) - 1u;                      /* floor(log2(n)) */
    unsigned t = (k >= 3u) ? (k - 3u) : 0u;
    seq_t pow_t = ((seq_t)1 << t);
    seq_t d     = n >> t;                                   /* floor(n/2^t) */

    /* delete = n - 2^t * (d-1)  (avoids underflow) */
    seq_t res = n - pow_t * (d - 1u);
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t ap = append_seq(n);
    return delete_seq(ap);
}
