/*
 * A306131  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3,
 *          4, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -3*a(n-2) +2*a(n-3) -29*a(n-4) -18*a(n-5)
 *            +80*a(n-6) * -17*a(n-7) +7*a(n-8) +102*a(n-9) -112*a(n-10)
 *            -65*a(n-11) +61*a(n-12) * -71*a(n-13) +49*a(n-14) +68*a(n-15)
 *            -72*a(n-16) -14*a(n-17) * +0*a(n-18) -16*a(n-19) -4*a(n-20) *
 *            with a(1)=1, a(2)=8, a(3)=13, a(4)=64, a(5)=240, a(6)=84...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[20] = {
        (seq_t)1ULL,
        (seq_t)8ULL,
        (seq_t)13ULL,
        (seq_t)64ULL,
        (seq_t)240ULL,
        (seq_t)842ULL,
        (seq_t)3302ULL,
        (seq_t)12740ULL,
        (seq_t)48468ULL,
        (seq_t)187112ULL,
        (seq_t)723138ULL,
        (seq_t)2786916ULL,
        (seq_t)10760740ULL,
        (seq_t)41576054ULL,
        (seq_t)160572130ULL,
        (seq_t)620281406ULL,
        (seq_t)2396512260ULL,
        (seq_t)9258772148ULL,
        (seq_t)35771455196ULL,
        (seq_t)138208433662ULL
    };

    static const int coeff[20] = {
        5, -3, 2, -29, -18, 80, -17, 7,
        102, -112, -65, 61, -71, 49, 68, -72,
        -14, 0, -16, -4
    };

    if (n <= 20)
        return init[n - 1];

    seq_t last[20];
    for (int i = 0; i < 20; ++i)
        last[i] = init[i];

    for (idx_t k = 21; k <= n; ++k) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int i = 0; i < 20; ++i) {
            seq_t prod = last[19 - i] *
                         (seq_t)(coeff[i] < 0 ? -coeff[i] : coeff[i]);
            if (coeff[i] > 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_t new_val = pos - neg;
        for (int i = 0; i < 19; ++i)
            last[i] = last[i + 1];
        last[19] = new_val;
    }
    return last[19];
}
