/*
 * A320188  Number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 7*d^2 = n.
 * https://oeis.org/A320188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A320188: Number of integer solutions to
 *     a^2 + 2*b^2 + 3*c^2 + 7*d^2 = n
 *
 * Formula: Count all integer quadruples (a,b,c,d) satisfying the equation.
 * The function below implements a direct enumeration, using the natural
 * symmetry of squares: each non‑zero term contributes a multiplicity of 2,
 * while zero contributes 1.  The implementation uses a binary
 * search‑based integer sqrt routine (isqrt_seq) that works with the
 * arbitrary‐precision seq_t type.
 */

static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, mid2;

    while (low <= high) {
        mid = (low + high) / 2;
        mid2 = mid * mid;
        if (mid2 == x) {
            return mid;
        }
        if (mid2 < x) {
            low = mid + 1;
        } else {
            /* prevent underflow: mid > 0 */
            if (mid == 0) {
                break;
            }
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t d = 0; ; ++d) {
        seq_t d_term = (seq_t)d * (seq_t)d * 7;
        if (d_term > n_seq) break;
        seq_t rem_d = n_seq - d_term;
        seq_t d_mult = (d == 0) ? 1 : 2;

        for (idx_t c = 0; ; ++c) {
            seq_t c_term = (seq_t)c * (seq_t)c * 3;
            if (c_term > rem_d) break;
            seq_t rem_dc = rem_d - c_term;
            seq_t c_mult = (c == 0) ? 1 : 2;

            for (idx_t b = 0; ; ++b) {
                seq_t b_term = (seq_t)b * (seq_t)b * 2;
                if (b_term > rem_dc) break;
                seq_t rem = rem_dc - b_term;
                seq_t a_val = isqrt_seq(rem);
                if (a_val * a_val == rem) {
                    seq_t a_mult = (a_val == 0) ? 1 : 2;
                    seq_t b_mult = (b == 0) ? 1 : 2;
                    count += a_mult * b_mult * c_mult * d_mult;
                }
            }
        }
    }

    return count;
}
