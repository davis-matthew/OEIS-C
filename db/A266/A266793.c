/*
 * A266793  Total number of ON (black) cells after n iterations of the "Rule
 *          61" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+7*(-1)^n-2*(-3+(-1)^n)*n+2*n^2)/4, for n>3; initial
 *            values: a(0)=1, a(1)=3, a(2)=5, a(3)=10
 * Range:     u8 n <= 10, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle small indices directly */
    switch (n) {
        case 0U: return (seq_t)1;
        case 1U: return (seq_t)3;
        case 2U: return (seq_t)5;
        case 3U: return (seq_t)10;
        default: break;
    }

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;            /* n^2 */

    seq_t numerator;
    if ((n & 1U) == 0U) { /* even n */
        numerator = 2 * n2 + 4 * n_seq + (seq_t)8;
    } else {              /* odd n (n>=5) */
        /* 2*n^2 + 8*n - 6 */
        numerator = (2 * n2 + 8 * n_seq) - (seq_t)6;
    }

    return numerator / (seq_t)4;
}
