/*
 * A295925  Number of bilaterally asymmetric 8-hoops with n symbols.
 * https://oeis.org/A295925
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/16)*(n^3-n^2-2)*(n^2+n+2)*(n+1)*(n-1)*n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;                       /* offset starts at 2 */
    seq_t n_seq = (seq_t)n;
    /* intermediate powers */
    seq_t n2 = n_seq * n_seq;                  /* n^2 */
    seq_t n3 = n2 * n_seq;                    /* n^3 */

    /* factors of the formula */
    seq_t f4 = n3 - n2 - (seq_t)2;             /* n^3 - n^2 - 2 */
    seq_t f5 = n2 + n_seq + (seq_t)2;          /* n^2 + n + 2 */

    seq_t factors[5] = {f4, f5, n_seq, n_seq - (seq_t)1, n_seq + (seq_t)1};

    seq_t res = (seq_t)1;
    seq_t divisor = (seq_t)16;                 /* division by 16 */

    for (int i = 0; i < 5; ++i) {
        /* pull powers of two from the current factor into the divisor */
        while ((factors[i] & (seq_t)1) == 0 && divisor > (seq_t)1) {
            factors[i] >>= 1;
            divisor >>= 1;
        }
        /* multiply and immediately reduce by any remaining factors of two */
        res *= factors[i];
        while (divisor > (seq_t)1 && (res & (seq_t)1) == 0) {
            res >>= 1;
            divisor >>= 1;
        }
    }

    /* any remaining divisor (if not fully cancelled) */
    if (divisor > (seq_t)1) res /= divisor;
    return res;
}
