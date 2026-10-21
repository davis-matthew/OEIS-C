/*
 * A263520  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 filled
 *          by rows with each element moved a city block distance of 0 or 1,
 *          and rows and columns in increasing lexicographic order.
 * https://oeis.org/A263520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) - 14*a(n-3) + 36*a(n-4) + 8*a(n-5) -
 *            36*a(n-6) - 2*a(n-7) + 13*a(n-8) + a(n-9) - a(n-10) (n ≥ 11,
 *            with a(1)=8, a(2)=35, a(3)=160, a(4)=660, a(5)=2651, a(6)=10350,
 *            a(7)=39807, a(8)=151463, a(9)=572454, a(10)=2153977)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[10] = {
        8ULL,       /* a(1) */
        35ULL,      /* a(2) */
        160ULL,     /* a(3) */
        660ULL,     /* a(4) */
        2651ULL,    /* a(5) */
        10350ULL,   /* a(6) */
        39807ULL,   /* a(7) */
        151463ULL,  /* a(8) */
        572454ULL,  /* a(9) */
        2153977ULL  /* a(10) */
    };

    if (n <= 10) return base[n - 1];

    seq_t buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = base[i];

    for (idx_t i = 11; i <= n; ++i) {
        /* compute positive and negative sums separately to avoid unsigned underflow */
        seq_t pos = (seq_t)7 * buf[9] + (seq_t)36 * buf[6]
                   + (seq_t)8 * buf[5] + (seq_t)13 * buf[2] + buf[1];
        seq_t neg = (seq_t)11 * buf[8] + (seq_t)14 * buf[7]
                   + (seq_t)36 * buf[4] + (seq_t)2 * buf[3] + buf[0];
        seq_t newval = pos - neg;

        /* shift buffer left and insert the new value */
        for (int k = 0; k < 9; ++k) buf[k] = buf[k + 1];
        buf[9] = newval;
    }

    return buf[9];
}
