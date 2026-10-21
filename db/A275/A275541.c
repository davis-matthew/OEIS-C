/*
 * A275541  (n)! + (n + 1)!!/(n + 1) - 2.
 * https://oeis.org/A275541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + (n-1)!! - 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* 0! + (-1)!! - 2 = 0 and 1! + 0!! - 2 = 0 */
        return (seq_t)0;
    }

    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t dblfact = (seq_t)1;
    idx_t i = n - 1;
    while (i > 0) {
        dblfact *= (seq_t)i;
        if (i <= 2) {
            break;
        }
        i -= 2;
    }

    return fact + dblfact - (seq_t)2;
}
