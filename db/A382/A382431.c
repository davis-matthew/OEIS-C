/*
 * A382431  Number of minimum dominating sets in the n-Goldberg graph.
 * https://oeis.org/A382431
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   piecewise according to the OEIS A382431 definition * a(5k) = 5 *
 *            a(5k-1) = 3*(5k-1) * a(5k-2) = 3*(5k-2)*(3k+11)/2 * a(5k-3) =
 *            9*(5k-3)*(k^2+11k-10)/2 * a(5k-4) =
 *            (5k-4)*(27k^3+594k^2+9k-742)/8 (k > 2) * a(6) = 1395 (special
 *            case for k = 2)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 545, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* undefined for n < 3 */

    idx_t r = n % 5;

    switch (r)
    {
        case 0:   /* a(5k) = 5 */
            return 5;

        case 4:   /* a(5k-1) = 3*(5k-1) */
        {
            idx_t k = (n + 1) / 5;
            seq_t k_s = (seq_t)k;
            return 3 * (5 * k_s - 1);
        }

        case 3:   /* a(5k-2) = 3*(5k-2)*(3k+11)/2 */
        {
            idx_t k = (n + 2) / 5;
            seq_t k_s = (seq_t)k;
            seq_t a_term = 5 * k_s - 2;
            seq_t b_term = 3 * k_s + 11;
            seq_t res = 3 * a_term;
            res *= b_term;
            res /= 2;
            return res;
        }

        case 2:   /* a(5k-3) = 9*(5k-3)*(k^2+11k-10)/2 */
        {
            idx_t k = (n + 3) / 5;
            seq_t k_s = (seq_t)k;
            seq_t a_term = 5 * k_s - 3;
            seq_t b_term = k_s * k_s + 11 * k_s - 10;
            seq_t res = 9 * a_term;
            res *= b_term;
            res /= 2;
            return res;
        }

        case 1:   /* a(5k-4) */
        {
            idx_t k = (n + 4) / 5;
            if (k == 2) {          /* special case for n = 6 */
                return 1395;
            }
            seq_t k_s = (seq_t)k;
            seq_t a_term = 5 * k_s - 4;
            seq_t poly = 27 * k_s * k_s * k_s
                       + 594 * k_s * k_s
                       + 9 * k_s
                       - 742;
            seq_t res = a_term * poly;
            res /= 8;
            return res;
        }

        default:
            return 0;  /* unreachable */
    }
}
