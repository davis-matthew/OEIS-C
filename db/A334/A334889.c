/*
 * A334889  a(n) = Sum_{k >= 0} f(d_k) * 3^k where Sum_{k >= 0} d_k * 8^k is
 *          the base 8 representation of n and f(k) = 0, 0, 0, 1, 2, 2, 2, 1
 *          for k = 0..7, respectively.
 * https://oeis.org/A334889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k≥0} f(d_k)·3^k where d_k are the base‑8 digits of n
 *            and f(k) = {0,0,0,1,2,2,2,1} for k = 0..7
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Mapping from an octal digit to the corresponding weight. */
    static const unsigned char f[8] = {0, 0, 0, 1, 2, 2, 2, 1};

    seq_t result = 0;       /* accumulated sum   */
    seq_t pow3   = 1;       /* current 3^k        */
    idx_t temp   = n;       /* work on a copy     */

    while (temp > 0) {
        unsigned digit = (unsigned)(temp & 7);   /* least significant octal digit */
        result += (seq_t)f[digit] * pow3;        /* add weighted digit            */
        temp >>= 3;                             /* move to next digit             */
        pow3 *= 3;                              /* advance power of 3             */
    }
    return result;
}
