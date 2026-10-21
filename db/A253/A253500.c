/*
 * A253500  Number of (6+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400*3^(n-3) + 2682*2^(n-1) + 16940 for n>6; Initial
 *            values: a(1)=19701, a(2)=22236, a(3)=29370, a(4)=40117,
 *            a(5)=63550, a(6)=113573
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 6) {
        static const seq_t init[6] = {
            19701, 22236, 29370, 40117, 63550, 113573
        };
        return init[n - 1];
    }
    seq_t p3 = pow_seq((seq_t)3, n - 3);
    seq_t p2 = pow_seq((seq_t)2, n - 1);
    return (seq_t)400 * p3 + (seq_t)2682 * p2 + (seq_t)16940;
}
