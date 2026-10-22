/*
 * A357624  Skew-alternating sum of the reversed n-th composition in standard
 *          order.
 * https://oeis.org/A357624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0}^{m-1} s_i * p_i where p_i are the successive
 *            differences of 1‑based positions of set bits of n, m is the
 *            number of set bits, and the sign s_i follows the pattern +, -,
 *            -, +, repeating every four parts
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t prev_pos = 0;       /* previous cut position (1‑based), starts at 0 */
    idx_t bit_index = 0;      /* current bit index in n (0‑based) */
    idx_t part_index = 0;     /* index of the current part for sign */

    idx_t temp = n;           /* use a copy for bit scanning */

    while (temp) {
        if (temp & 1ULL) {                /* bit is set */
            idx_t curr_pos = bit_index + 1;   /* 1‑based position of the bit */
            idx_t part = curr_pos - prev_pos; /* size of this part */

            /* sign pattern: +, -, -, + repeating (period 4) */
            idx_t mod = part_index & 3;
            seq_t sign = ((mod == 0) || (mod == 3)) ? (seq_t)1 : (seq_t)-1;

            sum += sign * (seq_t)part;

            prev_pos = curr_pos;
            part_index++;
        }

        temp >>= 1;
        bit_index++;
    }

    return sum;        /* zero when n == 0 (empty composition) */
}
