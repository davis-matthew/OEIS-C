/*
 * A390849  a(n) = Sum_{k=0..n} (k+1) * 2^k * 3^(n-k) * binomial(2*k,2*(n-k)).
 * https://oeis.org/A390849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 2^k * 3^(n-k) * binomial(2*k,
 *            2*(n-k)) Recurrent implementation:
 *            a(0)=1,a(1)=4,a(2)=24,a(3)=248,a(4)=1628,
 *            a(5)=11232,a(6)=77632,a(7)=513088. a(n) = 8*a(n-1) -16*a(n-3)
 *            -328*a(n-4) -96*a(n-5) +1728*a(n-7) -1296*a(n-8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(0)..a(7) */
    static const seq_t init[8] = {
        1,
        4,
        24,
        248,
        1628,
        11232,
        77632,
        513088
    };

    if (n < 8)
        return init[n];

    /* Ring buffer of the last 8 terms. */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        /* Retrieve needed previous terms */
        seq_t a1 = buf[(i - 1) & 7];
        seq_t a3 = buf[(i - 3) & 7];
        seq_t a4 = buf[(i - 4) & 7];
        seq_t a5 = buf[(i - 5) & 7];
        seq_t a7 = buf[(i - 7) & 7];
        seq_t a8 = buf[(i - 8) & 7];

        /* Compute next term with safe subtraction */
        seq_t positive = 8 * a1 + 1728 * a7;
        seq_t negative = 16 * a3 + 328 * a4 + 96 * a5 + 1296 * a8;
        buf[i & 7] = positive - negative;
    }

    return buf[n & 7];
}
