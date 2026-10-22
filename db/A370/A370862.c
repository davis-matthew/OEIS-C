/*
 * A370862  Numbers m such that c(0) <= c(1) >= c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer m such that in base‑3, count_0(m) <=
 *            count_1(m) >= count_2(m)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t desired = n;
    idx_t found   = 0;
    seq_t m = 0;

    while (found < desired) {
        ++m;
        seq_t temp = m;
        unsigned c0 = 0, c1 = 0, c2 = 0;

        if (temp == 0) {
            ++c0;
        } else {
            while (temp > 0) {
                unsigned digit = (unsigned)(temp % 3);
                if (digit == 0)
                    ++c0;
                else if (digit == 1)
                    ++c1;
                else
                    ++c2;
                temp /= 3;
            }
        }

        if (c0 <= c1 && c1 >= c2)
            ++found;
    }

    return m;
}
