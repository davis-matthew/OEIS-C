/*
 * A275112  Zero together with the partial sums of A064412.
 * https://oeis.org/A275112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^4 + 36*n^3 + 36*n^2 + 42*n + 11 + 3*(2*n-1)*(-1)^n
 *            - 8*(-1)^(((2*n-1 + (-1)^n))/4)) / 128
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 178, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the explicit offset value a(0)=0 */
    if (n == 0) {
        return 0;
    }

    /* Convert n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Powers of n */
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n2 * n2;       /* n^4 */

    /* Base polynomial 14*n^4 + 36*n^3 + 36*n^2 + 42*n + 11 */
    seq_t base = 14 * n4 + 36 * n3 + 36 * n2 + 42 * nn + 11;

    /* Parity helpers: (-1)^n and (-1)^((2n-1+(-1)^n)/4) */
    int sign   = (n & 1ULL) ? -1 : 1;           /* (-1)^n */
    int sign2  = ((n >> 1) & 1) ? -1 : 1;       /* (-1)^k where k = floor(n/2) */

    /* Positive and negative offsets to avoid signed arithmetic on seq_t */
    seq_t offsetPos = 0;
    seq_t offsetNeg = 0;

    /* 3*(2*n-1) term */
    seq_t two_n_minus_one = (seq_t)(2 * nn) - 1;        /* for n>0, always >=1 */
    seq_t three_times = 3 * two_n_minus_one;

    if (sign == 1) {
        offsetPos += three_times;
    } else {
        offsetNeg += three_times;
    }

    /* -8 * sign2 term */
    if (sign2 == 1) {
        offsetNeg += 8;
    } else {
        offsetPos += 8;
    }

    /* Assemble numerator: base + offsetPos - offsetNeg */
    seq_t numerator = base + offsetPos;
    numerator -= offsetNeg;

    /* Divide by 128 to obtain the term */
    return numerator / 128;
}
