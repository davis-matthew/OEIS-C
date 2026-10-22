/*
 * A319258  a(n) = 1 + 2*3 + 4 + 5*6 + 7 + 8*9 + 10 + 11*12 + ... + (up to n).
 * https://oeis.org/A319258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{r=1}^{⌊n/3⌋} (9 r² – 2) + + (n%3==1? (3⌊n/3⌋+1) : 0)
 *            + + (n%3==2? (3⌊n/3⌋+1)+(3⌊n/3⌋+2) : 0)
 * Range:     u8 n <= 11, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 is the only required range; behaviour for n < 1 is irrelevant */
    idx_t k = n / 3;
    idx_t rem = n % 3;
    seq_t K = (seq_t)k;
    seq_t full = 0;

    if (k > 0) {
        /* full = (3/2) * k(k+1)(2k+1) – 2k */
        seq_t t1 = K * (K + 1);        /* k(k+1)           */
        seq_t t2 = t1 * (2 * K + 1);   /* k(k+1)(2k+1)     */
        seq_t temp = (t2 * 3) / 2;     /* (3/2) * t2       */
        full = temp - 2 * K;           /* subtract 2k       */
    }

    if (rem == 1) {
        full += 3 * K + 1;
    } else if (rem == 2) {
        full += (3 * K + 1) + (3 * K + 2);
    }

    return full;
}
