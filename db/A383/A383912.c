/*
 * A383912  Expansion of (1+x) * (1+2*x)/((1-x) * (1-2*x) * (1-3*x)).
 * https://oeis.org/A383912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A383912
 *  Formula: a(n) = 10*3^n - 3*2^(n+2) + 3          (a(0)=1)
 *  The function works for all supported seq_t sizes.
 *  For small seq_t (≤64‑bit) the calculation is performed with
 *  a uint64_t intermediate to avoid overflow of the small type.
 *  For larger seq_t (arbitrary‑precision) standard seq_t arithmetic
 *  is used.
 */

#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* If seq_t is at most 64 bits, perform the computation
       using a larger intermediate type. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        uint64_t p3   = (uint64_t)pow_uint(3, n);
        uint64_t p2   = (uint64_t)pow_uint(2, n + 2);
        uint64_t res  = 10ULL * p3 - 3ULL * p2 + 3ULL;
        return (seq_t)res;   /* res fits into seq_t because the exact a(n) fits */
    }

    /* For larger seq_t, rely on seq_t arithmetic (or overloaded
       operators for arbitrary‑precision types). */
    seq_t p3 = pow_uint(3, n);      /* 3^n */
    seq_t p2 = pow_uint(2, n + 2);  /* 2^(n+2) */
    seq_t res = 10 * p3;            /* 10*3^n  */
    res -= 3 * p2;                  /* -3*2^(n+2) */
    res += 3;                       /* +3 */
    return res;
}
