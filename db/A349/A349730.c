/*
 * A349730  Row sums of A349728.
 * https://oeis.org/A349730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 Recurrence for a(n) (n ≥ 6): a(n) = [
 *            (n−1)(n²+11n−19)a(n−1) −2(6n³−n²−68n+93)a(n−2)
 *            +2(25n³−130n²+146n+65)a(n−3) −4(20n³−182n²+531n−483)a(n−4)
 *            +8(n−4)(2n−9)²a(n−5) ] / [n(n−1)]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first six known terms */
    static const seq_t init[6] = {1, 1, 4, 16, 66, 284};

    if (n < 6)
        return init[n];

#ifdef __cplusplus
    /* C++ build – use arbitrary‑precision arithmetic in seq_t directly */
    seq_t buf[6] = {1, 1, 4, 16, 66, 284};          /* a(n‑6)…a(n‑1)  for i=6 */
    for (idx_t i = 6; i <= n; ++i)
    {
        const seq_t a_nm1 = buf[5];
        const seq_t a_nm2 = buf[4];
        const seq_t a_nm3 = buf[3];
        const seq_t a_nm4 = buf[2];
        const seq_t a_nm5 = buf[1];

        /* term 1 : (n-1)(n^2+11n-19)a(n-1) */
        seq_t term1 = (seq_t)(i - 1) *
                      (((seq_t)i * (seq_t)i + 11 * (seq_t)i - 19)) * a_nm1;

        /* term 2 : -2(6n^3-n^2-68n+93)a(n-2) */
        seq_t coeff2 = 2 *
                       ((seq_t)6 * (seq_t)i * (seq_t)i * (seq_t)i -
                        (seq_t)i * (seq_t)i -
                        68 * (seq_t)i + 93);
        seq_t term2 = coeff2 * a_nm2;

        /* term 3 : +2(25n^3-130n^2+146n+65)a(n-3) */
        seq_t coeff3 = 2 *
                       ((seq_t)25 * (seq_t)i * (seq_t)i * (seq_t)i -
                        130 * (seq_t)i * (seq_t)i +
                        146 * (seq_t)i + 65);
        seq_t term3 = coeff3 * a_nm3;

        /* term 4 : -4(20n^3-182n^2+531n-483)a(n-4) */
        seq_t coeff4 = 4 *
                       ((seq_t)20 * (seq_t)i * (seq_t)i * (seq_t)i -
                        182 * (seq_t)i * (seq_t)i +
                        531 * (seq_t)i - 483);
        seq_t term4 = coeff4 * a_nm4;

        /* term 5 : +8(n-4)(2n-9)^2a(n-5) */
        seq_t t2 = (seq_t)(2 * (seq_t)i - 9);
        seq_t coeff5 = 8 * (seq_t)(i - 4) * t2 * t2;
        seq_t term5 = coeff5 * a_nm5;

        /* numerator = term1 - term2 + term3 - term4 + term5 */
        seq_t numerator = term1 - term2 + term3 - term4 + term5;
        seq_t denom     = (seq_t)i * ((seq_t)(i - 1));

        seq_t a_i = numerator / denom;

        /* shift window: drop oldest, push newest */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = a_i;
    }
    return buf[5];
#else
    /* C build – use 128‑bit intermediates to avoid overflow */
    seq_t buf[6] = {1, 1, 4, 16, 66, 284};
    for (idx_t i = 6; i <= n; ++i)
    {
        unsigned __int128 a_nm1 = buf[5];
        unsigned __int128 a_nm2 = buf[4];
        unsigned __int128 a_nm3 = buf[3];
        unsigned __int128 a_nm4 = buf[2];
        unsigned __int128 a_nm5 = buf[1];

        unsigned __int128 coeff1 = (unsigned __int128)(i - 1) *
                                  (((unsigned __int128)i * i + 11 * (unsigned __int128)i - 19));
        unsigned __int128 coeff2 = 2 *
                                  (((unsigned __int128)6 * i * i * i) -
                                   (unsigned __int128)i * i -
                                   68 * (unsigned __int128)i + 93);
        unsigned __int128 coeff3 = 2 *
                                  (((unsigned __int128)25 * i * i * i) -
                                   130 * (unsigned __int128)i * i +
                                   146 * (unsigned __int128)i + 65);
        unsigned __int128 coeff4 = 4 *
                                  (((unsigned __int128)20 * i * i * i) -
                                   182 * (unsigned __int128)i * i +
                                   531 * (unsigned __int128)i - 483);
        unsigned __int128 coeff5 = 8 * (unsigned __int128)(i - 4) *
                                  ((unsigned __int128)(2 * i - 9) * (unsigned __int128)(2 * i - 9));

        unsigned __int128 numerator = coeff1 * a_nm1
                                    - coeff2 * a_nm2
                                    + coeff3 * a_nm3
                                    - coeff4 * a_nm4
                                    + coeff5 * a_nm5;
        unsigned __int128 denom = (unsigned __int128)i * (i - 1);
        seq_t a_i = (seq_t)(numerator / denom);

        /* shift window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = a_i;
    }
    return buf[5];
#endif
}
