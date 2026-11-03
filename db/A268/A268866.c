/*
 * A268866  Records in A268865.
 * https://oeis.org/A268866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n even: (4^(n+1)+2)/3 ; n odd: (7*4^(n-1)+2)/3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n */
        seq_t v = pow4(n + 1);         /* 4^(n+1) */
        v += (seq_t)2;
        v /= (seq_t)3;
        return v;
    } else {                            /* odd n */
        seq_t v = pow4(n - 1);         /* 4^(n-1) */
        v *= (seq_t)7;
        v += (seq_t)2;
        v /= (seq_t)3;
        return v;
    }
}
