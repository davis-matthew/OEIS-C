/*
 * A304973  Number of achiral color patterns (set partitions) for a row or
 *          loop of length n using exactly 3 colors (sets).
 * https://oeis.org/A304973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------
 * Sequence A304973:  Number of achiral color patterns (set partitions)
 * for a row or loop of length n using exactly 3 colors.
 *
 * Formula (from OEIS):
 *     a(n) = [n even] * ( 2*S2(n/2+1,3) - 2*S2(n/2,3) )
 *           + [n odd ] * (   S2((n+3)/2,3) -   S2((n+1)/2,3) )
 *
 * where S2(m,3) is the Stirling number of the second kind {m,3}.
 *
 * For k = 3 the Stirling number has the closed form
 *     S2(m,3) = (3^m - 3*2^m + 3) / 6   for m ≥ 3
 *             = 0                        for m < 3
 *
 * The implementation uses a small exponentiation routine that works
 * for any seq_t type (built‑in or arbitrary‑precision).
 *--------------------------------------------------------------------*/

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t S2_of_3(idx_t m) {
    if (m < 3) {
        return (seq_t)0;
    }
    seq_t pow3 = pow_seq((seq_t)3, m);
    seq_t pow2 = pow_seq((seq_t)2, m);
    seq_t three_pow2 = (seq_t)3 * pow2;                /* 3 * 2^m   */
    seq_t num = pow3 - three_pow2 + (seq_t)3;         /* 3^m - 3*2^m + 3 */
    return num / (seq_t)6;                           /* division exact */
}

seq_t a(idx_t n) {
    if ((n & 1U) == 0U) { /* n even */
        idx_t m1 = n / 2 + 1;
        idx_t m2 = n / 2;
        seq_t s1 = S2_of_3(m1);
        seq_t s2 = S2_of_3(m2);
        return (seq_t)2 * s1 - (seq_t)2 * s2;
    } else { /* n odd */
        idx_t m1 = (n + 3) / 2;
        idx_t m2 = (n + 1) / 2;
        seq_t s1 = S2_of_3(m1);
        seq_t s2 = S2_of_3(m2);
        return s1 - s2;
    }
}
