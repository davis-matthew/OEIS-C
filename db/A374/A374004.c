/*
 * A374004  a(n) = (1 + (n+3)^2 - (n-4)*(-1)^n)/2.
 * https://oeis.org/A374004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+3)^2 - (n-4)*(-1)^n)/2
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use only seq_t arithmetic, no signed overflow. */
    seq_t base = (seq_t)n;                /* n */
    seq_t t = base + 3;                   /* n+3 */
    seq_t term1 = t * t;                  /* (n+3)^2 */
    seq_t term = term1 + 1;               /* 1 + (n+3)^2 */

    seq_t abs_diff;
    if (n >= 4) {                         /* |n-4| = n-4 */
        abs_diff = base - 4;
        if ((n & 1) == 0) {              /* even n: (n-4) */
            term -= abs_diff;            /* term - (n-4) */
        } else {                          /* odd n: -(n-4) */
            term += abs_diff;            /* term + (n-4) */
        }
    } else {                              /* n < 4: |n-4| = 4-n */
        abs_diff = 4 - base;
        if ((n & 1) == 0) {              /* even n: -(4-n) */
            term += abs_diff;            /* term + (4-n) */
        } else {                          /* odd n: (4-n) */
            term -= abs_diff;            /* term - (4-n) */
        }
    }

    term /= 2;                            /* final division by 2 */
    return term;
}
