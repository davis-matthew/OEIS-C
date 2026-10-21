/*
 * A256577  Sum_{k>=0} (d_k)^(k+1)*10^k, where Sum_{k>=0} (d_k)*10^k is the
 *          decimal expansion of n.
 * https://oeis.org/A256577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = Sum_{k>=0} (d_k)^(k+1) * 10^k where n = Sum_{k>=0} d_k *
 *            10^k (decimal digits d_k) For n = 0, a(n) = 0
 * Range:     u8 n <= 55, u16 n <= 899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result  = 0;
    seq_t pow10   = 1;          /* 10^k */
    idx_t pos     = 0;          /* current digit position */
    idx_t tmp     = n;          /* copy of index for digit extraction */

    /* If n == 0, result remains 0 */
    while (tmp > 0) {
        idx_t digit = tmp % 10;      /* current decimal digit */
        tmp /= 10;

        /* Compute digit^(pos+1) */
        seq_t power = 1;
        for (idx_t i = 0; i < pos + 1; ++i) {
            power *= (seq_t)digit;
        }

        result += power * pow10;
        pow10  = pow10 * ((seq_t)10);
        ++pos;
    }
    return result;
}
