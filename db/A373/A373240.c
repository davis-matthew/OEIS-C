/*
 * A373240  Greatest common divisor of the product of terms in positive
 *          Pythagorean n-tuples, over all possible such tuples.
 * https://oeis.org/A373240
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^α(n) * 3^β(n) for n ≥ 4. α(n) is given by n mod 8: 0 →
 *            3, 1 → 2, 2 → 0, 3 → 2, 4 → 2, 5 → 1, 6 → 3, 7 → 1. β(n) = 1 if
 *            n mod 3 ≠ 2, otherwise 0. (Special case: a(3) = 60.)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                /* offset */
        return 1;
    if (n == 3)                /* special case */
        return 60;

    int alpha;
    switch ((int)(n & 7u)) {   /* n mod 8 */
    case 0: alpha = 3; break;
    case 1: alpha = 2; break;
    case 2: alpha = 0; break;
    case 3: alpha = 2; break;
    case 4: alpha = 2; break;
    case 5: alpha = 1; break;
    case 6: alpha = 3; break;
    case 7: alpha = 1; break;
    default: alpha = 0; /* unreachable */
    }

    int beta = (n % 3 != 2) ? 1 : 0;

    seq_t res = 1;
    for (int i = 0; i < alpha; ++i)
        res *= 2;
    if (beta)
        res *= 3;

    return res;
}
