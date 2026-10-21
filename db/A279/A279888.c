/*
 * A279888  a(n) = Sum_{k=1..n-1} sigma_3(k)*sigma_5(n-k).
 * https://oeis.org/A279888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*sigma_9(n)-21*sigma_5(n)+10*sigma_3(n))/5040
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 273, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sigma_k for k = 3,5,9 by enumerating divisors once. */
seq_t a(idx_t n)
{
    /* If seq_t is a native integer type (e.g. uint64_t), intermediate
       values up to the 128‑bit range are computed in unsigned __int128.
       The final result is then cast back to seq_t. */
    unsigned __int128 sum3 = 0;
    unsigned __int128 sum5 = 0;
    unsigned __int128 sum9 = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* compute powers of d */
            unsigned __int128 d2 = (unsigned __int128)d * d;
            unsigned __int128 d3 = d2 * d;
            unsigned __int128 d4 = d3 * d;
            unsigned __int128 d5 = d4 * d;
            unsigned __int128 d9 = d5 * d4; /* d^5 * d^4 = d^9 */

            sum3 += d3;
            sum5 += d5;
            sum9 += d9;

            idx_t other = n / d;
            if (other != d) {
                unsigned __int128 od = other;
                unsigned __int128 od2 = od * od;
                unsigned __int128 od3 = od2 * od;
                unsigned __int128 od4 = od3 * od;
                unsigned __int128 od5 = od4 * od;
                unsigned __int128 od9 = od5 * od4;

                sum3 += od3;
                sum5 += od5;
                sum9 += od9;
            }
        }
    }

    /* numerator = 11*sum9 - 21*sum5 + 10*sum3 */
    unsigned __int128 num =
        (unsigned __int128)11 * sum9
        - (unsigned __int128)21 * sum5
        + (unsigned __int128)10 * sum3;

    unsigned __int128 res = num / 5040;      /* integer division */

    return (seq_t)res;
}
