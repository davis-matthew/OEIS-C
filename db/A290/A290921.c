/*
 * A290921  p-INVERT of the positive integers, where p(S) = (1 - S)^6.
 * https://oeis.org/A290921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 141*a(n-2) + 630*a(n-3) - 1770*a(n-4) * +
 *            3258*a(n-5) - 3989*a(n-6) + 3258*a(n-7) - 1770*a(n-8) * +
 *            630*a(n-9) - 141*a(n-10) + 18*a(n-11) - a(n-12); * Initial
 *            values: * a(0)=6, a(1)=33, a(2)=158, a(3)=696, a(4)=2886, *
 *            a(5)=11425, a(6)=43590, a(7)=161355, a(8)=582340, * a(9)=20...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t init[12] = {
        (seq_t)6,      (seq_t)33,      (seq_t)158,
        (seq_t)696,    (seq_t)2886,    (seq_t)11425,
        (seq_t)43590,  (seq_t)161355,  (seq_t)582340,
        (seq_t)2056818,(seq_t)7130388, (seq_t)24319054
    };

    if (n < 12) return init[n];

    /* Working variables for the last 12 terms
       a0 = a(k-12), a1 = a(k-11), ..., a11 = a(k-1) */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];
    seq_t a8 = init[8];
    seq_t a9 = init[9];
    seq_t a10 = init[10];
    seq_t a11 = init[11];

    idx_t i;
    for (i = 12; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = (seq_t)18 * a11
                  + (seq_t)630 * a9
                  + (seq_t)3258 * a7
                  + (seq_t)3258 * a5
                  + (seq_t)630 * a3
                  + (seq_t)18 * a1;

        /* Negative contributions */
        seq_t neg = (seq_t)141 * a10
                  + (seq_t)1770 * a8
                  + (seq_t)3989 * a6
                  + (seq_t)1770 * a4
                  + (seq_t)141 * a2
                  + a0;

        /* Next term (pos - neg, positive by definition) */
        seq_t next = pos - neg;

        /* Shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = a9;
        a9 = a10;
        a10 = a11;
        a11 = next;
    }

    return a11;
}
