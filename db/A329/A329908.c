/*
 * A329908  Number of oriented rational links with crossing number n.
 * https://oeis.org/A329908
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-1)+1)/3 + 2^(n/2-1) if n even; * a(n) =
 *            (2^(n-1)+2^((n-1)/2)-2)/3 if n odd and n==1 mod 4; * a(n) =
 *            (2^(n-1)+2^((n-1)/2))/3 if n odd and n==3 mod 4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_u.h"

seq_t a(idx_t n) {
    if (n < 2) return 0; /* undefined for n < 2, return 0 */
    seq_t term1 = pow2_u(n - 1);        /* 2^(n-1) */
    if ((n & 1ULL) == 0ULL) {           /* even n */
        seq_t part1 = (term1 + 1) / 3;  /* (2^(n-1)+1)/3 */
        seq_t part2 = pow2_u(n / 2 - 1);/* 2^(n/2-1) */
        return part1 + part2;
    } else {                            /* odd n */
        if ((n & 3ULL) == 1ULL) {       /* n ≡ 1 (mod 4) */
            seq_t pow_half = pow2_u((n - 1) / 2); /* 2^((n-1)/2) */
            seq_t part = (term1 + pow_half - 2) / 3;
            return part;
        } else { /* n ≡ 3 (mod 4) */
            seq_t pow_half = pow2_u((n - 1) / 2);
            seq_t part = (term1 + pow_half) / 3;
            return part;
        }
    }
}
