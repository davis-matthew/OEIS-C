/*
 * A250538  Number of (n+1)X(2+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n‑1) – 415*a(n‑2) + 3514*a(n‑3) – 20386*a(n‑4) +
 *            85924*a(n‑5) – 272198*a(n‑6) + 661180*a(n‑7) – 1244717*a(n‑8) +
 *            1822478*a(n‑9) – 2068955*a(n‑10) + 1802474*a(n‑11) –
 *            1181760*a(n‑12) + 563888*a(n‑13) – 184752*a(n‑14) +
 *            37152*a(n‑15) – 3456*a(n‑16); Initial values: a(1)=1080,
 *            a(2)=10704...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[16] = {
        1080ULL, 10704ULL, 79366ULL, 491650ULL, 2701872ULL, 13657024ULL,
        64993652ULL, 295871112ULL, 1302924116ULL, 5595784456ULL,
        23580939154ULL, 97951328382ULL, 402468021792ULL, 1640200686104ULL,
        6643744925312ULL, 26790344223060ULL
    };
    static const unsigned int coeff[16] = {
        30U, 415U, 3514U, 20386U, 85924U, 272198U, 661180U, 1244717U,
        1822478U, 2068955U, 1802474U, 1181760U, 563888U, 184752U,
        37152U, 3456U
    };

    if (n <= 16) {
        return init[n - 1];
    }

    seq_t buf[16];
    for (unsigned int i = 0; i < 16; ++i) {
        buf[i] = init[i];
    }

    for (idx_t cur = 17; cur <= n; ++cur) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (unsigned int k = 0; k < 16; ++k) {
            seq_t prod = (seq_t)coeff[k] * buf[15 - k];
            if ((k & 1U) == 0) {          /* even k → positive coefficient */
                sum_pos += prod;
            } else {                       /* odd k → negative coefficient */
                sum_neg += prod;
            }
        }
        seq_t next = sum_pos - sum_neg;   /* sum_pos ≥ sum_neg for all n */

        /* shift buffer left by one and append new value */
        for (unsigned int i = 0; i < 15; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[15] = next;
    }

    return buf[15];
}
