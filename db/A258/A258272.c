/*
 * A258272  The smallest amount which cannot be made with fewer than n British
 *          coins.
 * https://oeis.org/A258272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest amount that requires at least n UK coins
 *            (1,2,5,10,20,50,100,200). For n >= 7 a(n) = 200*n - 1212; For n
 *            < 7 the value is determined by a short search using the greedy
 *            coin‑change algorithm
 * Range:     u8 n <= 7, u16 n <= 333, u32 n <= 5006, u64 n <= 5006,
 *            u128 n <= 5006, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t denoms[8] = {200, 100, 50, 20, 10, 5, 2, 1};

    if (n >= 7)
        return (seq_t)200 * (seq_t)n - (seq_t)1212;

    /* n < 7 : search from 1 upwards for the first amount whose minimal
       coin count is at least n.  Greedy gives the optimal count for
       this canonical coin system. */
    seq_t amt = 1;
    while (1) {
        seq_t rem = amt;
        seq_t cnt = 0;
        for (idx_t i = 0; i < 8; ++i) {
            seq_t d = denoms[i];
            seq_t q = rem / d;
            cnt += q;
            rem -= q * d;
        }
        if (cnt >= (seq_t)n)
            return amt;
        ++amt;
    }
}
