/*
 * A382657  Number of minimum total dominating sets in the n-Goldberg graph.
 * https://oeis.org/A382657
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k) = 10 a(5k-1) = (5k-1)*(k+13)*(k^2+2k+24)/12 a(5k-2) =
 *            (5k-2)*(k^7+70k^6+1750k^5+39340k^4+525889k^3+2944270k^2+15922920k+12216960)/20160
 *            a(5k-3) = (5k-3)*(k+2) a(5k-4) =
 *            (5k-4)*(k^5+35k^4+365k^3+5485k^2+21114k+16200)/360 Special
 *            values: a(3)=16, a(4)=277, a(6)=2386, a(8)=33301
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t r = n % 5;
    switch (r) {
        case 0: /* 5k */
            return 10;

        case 1: /* 5k-4 */
            if (n == 6) return 2386;   /* k = 2, outside the valid range for the formula */
            {
                idx_t k = (n + 4) / 5;
                seq_t kk = (seq_t)k;
                seq_t k2 = kk * kk;
                seq_t k3 = k2 * kk;
                seq_t k4 = k3 * kk;
                seq_t k5 = k4 * kk;
                seq_t poly = k5
                           + 35 * k4
                           + 365 * k3
                           + 5485 * k2
                           + 21114 * kk
                           + 16200;
                seq_t val = (seq_t)(5 * k - 4) * poly / 360;
                return val;
            }

        case 2: /* 5k-3 */
            {
                idx_t k = (n + 3) / 5;
                return (seq_t)(5 * k - 3) * (seq_t)(k + 2);
            }

        case 3: /* 5k-2 */
            if (n == 3) return 16;
            if (n == 8) return 33301;   /* k = 2, outside the valid range for the formula */
            {
                idx_t k = (n + 2) / 5;
                seq_t kk = (seq_t)k;
                seq_t k2 = kk * kk;
                seq_t k3 = k2 * kk;
                seq_t k4 = k3 * kk;
                seq_t k5 = k4 * kk;
                seq_t k6 = k5 * kk;
                seq_t k7 = k6 * kk;
                seq_t poly = k7
                           + 70 * k6
                           + 1750 * k5
                           + 39340 * k4
                           + 525889 * k3
                           + 2944270 * k2
                           + 15922920 * kk
                           + 12216960;
                seq_t val = (seq_t)(5 * k - 2) * poly / 20160;
                return val;
            }

        case 4: /* 5k-1 */
            if (n == 4) return 277;   /* k = 1, outside the valid range for the formula */
            {
                idx_t k = (n + 1) / 5;
                seq_t kk = (seq_t)k;
                seq_t k2 = kk * kk;
                seq_t val = (seq_t)(5 * k - 1) * (kk + 13) * (k2 + 2 * kk + 24) / 12;
                return val;
            }
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
