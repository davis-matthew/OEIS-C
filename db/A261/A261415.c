/*
 * A261415  The maximal midpoint-free set Z_7^{+}{0,1,3}.
 * https://oeis.org/A261415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number whose base‑7 digits are obtained by writing
 *            (n−1) in base‑3 and replacing each digit 0,1,2 by 0,1,3. That
 *            is, a(n)=∑ (digit_i==2?3:digit_i)*7^i for the base‑3 digits of
 *            n−1
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n){
    idx_t m = n - 1;             /* use n−1 because a(1)=0 */
    seq_t value = 0;             /* accumulated result */
    seq_t pow7  = 1;             /* current power of 7 (7^i) */

    /* Process digits of m in base 3 (least significant first) */
    while (m) {
        idx_t d = m % 3;                  /* base‑3 digit */
        seq_t mapped = (d == 2) ? 3 : d;   /* map 0→0, 1→1, 2→3 */
        value += mapped * pow7;            /* add contribution of this digit */
        m /= 3;                            /* move to next digit */
        pow7 *= 7;                        /* update 7^i */
    }

    return value;
}
