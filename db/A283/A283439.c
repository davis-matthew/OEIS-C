/*
 * A283439  Hankel transform of A033434.
 * https://oeis.org/A283439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3*k) = (-1)^k*(k + 1)*(2*k + 1) a(3*k + 1) = -(-1)^k*(k +
 *            1)*(2*k + 3) a(3*k + 2) = -(-1)^k*(2*k + 3)^2
 * Range:     i8 n <= 16, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k_idx = n / 3;
    idx_t r = n % 3;
    seq_t k = (seq_t)k_idx;
    seq_t sign = ((k_idx & 1) == 0) ? (seq_t)1 : (seq_t)-1;
    seq_t result;
    switch (r)
    {
        case 0:
            result = sign * (k + 1) * (2 * k + 1);
            break;
        case 1:
            result = -sign * (k + 1) * (2 * k + 3);
            break;
        case 2:
        {
            seq_t two_k_plus_3 = 2 * k + 3;
            result = -sign * two_k_plus_3 * two_k_plus_3;
            break;
        }
        default:
            result = 0; /* unreachable */
    }
    return result;
}
