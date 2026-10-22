/*
 * A389099  Array read by ascending antidiagonals: A(n, k) = k / gcd(n^k, k).
 * https://oeis.org/A389099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k / gcd(i^k , k) where let T(s) = s*(s+1)/2, find s such
 *            that T(s) <= n < T(s+1). offset = n - T(s); i = s - offset; k =
 *            offset. A(i,k) = k / gcd(i^k, k). (and A(i,0)=0)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Find s such that T(s) <= n < T(s+1) */
    idx_t s = 0;
    while ( ( (__int128)((s + 1) * (s + 2)) ) / 2 <= n ) {
        s++;
    }

    /* triangular number T(s) and offset inside this diagonal */
    idx_t base = (idx_t)( ( (__int128)s * (s + 1) ) / 2 );
    idx_t offset = n - base;          /* 0 <= offset <= s */

    idx_t i = s - offset;             /* row index */
    idx_t k = offset;                 /* column index */

    if (k == 0)                      /* A(i,0) = 0 by definition */
        return (seq_t)0;

    seq_t i_seq = (seq_t)i;
    seq_t k_seq = (seq_t)k;

    /* Compute i^k mod k using the provided mod_pow helper */
    seq_t powmod_val = mod_pow(i_seq, (uint64_t)k, k_seq);

    /* Greatest common divisor */
    seq_t g = gcd(powmod_val, k_seq);

    return k_seq / g;
}
