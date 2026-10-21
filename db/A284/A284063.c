/*
 * A284063  Numbers whose smallest decimal digit is 2.
 * https://oeis.org/A284063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th positive integer whose smallest decimal digit is 2. */
seq_t a(idx_t n)
{
    /* Precompute powers of 8 and 7 up to exponent 64 (enough for all practical n). */
    static seq_t pow8[65];
    static seq_t pow7[65];
    static int initialized = 0;
    if (!initialized) {
        pow8[0] = (seq_t)1;
        pow7[0] = (seq_t)1;
        for (int i = 1; i < 65; ++i) {
            pow8[i] = pow8[i - 1] * (seq_t)8;
            pow7[i] = pow7[i - 1] * (seq_t)7;
        }
        initialized = 1;
    }

    /* Find the number of digits k of the desired term. */
    idx_t k = 1;
    seq_t cum = (seq_t)0;
    while (1) {
        seq_t add = pow8[k] - pow7[k];
        if (cum + add >= (seq_t)n)
            break;
        cum += add;
        ++k;
    }

    /* m is the 0‑based index among the k‑digit terms. */
    seq_t m = (seq_t)n - cum - (seq_t)1;

    seq_t result = (seq_t)0;
    int seen2 = 0;  /* have we already seen a digit 2? */
    for (int pos = 0; pos < (int)k; ++pos) {
        int rem = (int)(k - pos - 1);    /* remaining positions after this one */
        for (int d = 0; d < 8; ++d) {    /* d in [0,7] corresponds to actual digit d+2 */
            seq_t cnt;
            if (seen2) {
                cnt = pow8[rem];
            } else {
                if (d == 0) {
                    cnt = pow8[rem];
                } else {
                    cnt = pow8[rem] - pow7[rem];
                }
            }

            if (m < cnt) {
                result = result * (seq_t)10 + (seq_t)(d + 2);
                if (d == 0)
                    seen2 = 1;
                break;
            } else {
                m -= cnt;
            }
        }
    }
    return result;
}
