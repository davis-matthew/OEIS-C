/*
 * A356133  Complement of A026430.
 * https://oeis.org/A356133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - A001285(n-1) where A001285(k) = 1 + Thue-Morse(k) =
 *            1 + parity_of_ones(k), i.e. 1 if k has an even number of 1-bits,
 *            2 otherwise. Thus a(n) = 3*n - 1 - parity_of_ones(n-1)
 * Range:     u8 n <= 85, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int parity_uint64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_parityll(x);
#else
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
#endif
}

seq_t a(idx_t n)
{
    /* NOTE: the sequence has offset 1; behaviour for n==0 is undefined
       but we return 0 for completeness. */
    if (n == 0) return 0;

    int p = parity_uint64(n - 1);          /* parity of 1-bits of n-1 */
    seq_t res = (seq_t)3 * (seq_t)n;       /* 3*n in seq_t */
    res -= (seq_t)1;                       /* subtract 1 */
    res -= (seq_t)p;                       /* subtract parity (0 or 1) */
    return res;
}
