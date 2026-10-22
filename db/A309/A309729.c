/*
 * A309729  Expansion of Sum_{k>=1} x^k/(1 - x^k - 2*x^(2*k)).
 * https://oeis.org/A309729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * sum_{d|n} (2^d - (-1)^d)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses big arithmetic for arbitrary-size seq_t.
 * For 64‑bit builds it switches to an unsigned __int128 intermediate
 * to avoid overflow for d up to 127 (enough for the 64‑bit case).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t divisor_term(idx_t d)
{
    /* For 32, 64 and 128‑bit built‑ins use a 128‑bit intermediate
     * when the divisor is not too large.  This covers all cases where
     * the result still fits in the native type. */
    if (sizeof(seq_t) <= 8 && d <= 127U) {
        unsigned __int128 p = (unsigned __int128)1 << d;   /* 2^d */
        if (d & 1U)   /* d odd: (+1) */
            p += (unsigned __int128)1;
        else          /* d even: (-1) */
            p -= (unsigned __int128)1;
        return (seq_t)(p / 3);
    }

    /* Fallback: use the generic big_pow routine for larger types
     * or when 128‑bit shifts would overflow.  This path is used
     * for arbitrary‑precision builds and for 64‑bit builds with
     * very large divisors (which would overflow the 64‑bit type
     * anyway). */
    seq_t p = big_pow((seq_t)2, d);   /* 2^d */
    if (d & 1U)   /* d odd: (+1) */
        p += (seq_t)1;
    else          /* d even: (-1) */
        p -= (seq_t)1;
    return p / (seq_t)3;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n / i; ++i) {
        if (n % i == (idx_t)0) {
            sum += divisor_term(i);
            idx_t j = n / i;
            if (j != i)
                sum += divisor_term(j);
        }
    }
    return sum;
}
