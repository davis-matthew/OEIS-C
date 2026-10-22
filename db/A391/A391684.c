/*
 * A391684  Number of divisors d of n which also divide (n/d)^2 - 1.
 * https://oeis.org/A391684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = # { d | n : d | (n/d)^2 - 1 } via divisor enumeration.
   Uses modular arithmetic to avoid overflow. */
seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;            /* the paired divisor */
            /* check divisor d */
            if (d == 1) {
                ++count;                /* 1 always works */
            } else {
                idx_t r = e % d;
                __uint128_t prod = ( __uint128_t ) r * r;
                if ((uint64_t)(prod % d) == 1) ++count;   /* r^2 ≡ 1 (mod d) */
            }
            /* check paired divisor e, if distinct */
            if (e != d) {
                if (e == 1) {
                    ++count;            /* e == 1 case */
                } else {
                    idx_t r2 = d % e;
                    __uint128_t prod2 = ( __uint128_t ) r2 * r2;
                    if ((uint64_t)(prod2 % e) == 1) ++count; /* r2^2 ≡ 1 (mod e) */
                }
            }
        }
    }
    return count;
}
