/*
 * A302255  Total domination number of the n-antiprism graph.
 * https://oeis.org/A302255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + 4 for n ≥ 7, with initial values a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=2, a(4)=3, a(5)=4, a(6)=4
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[7] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)3, (seq_t)4, (seq_t)4
    };
    if (n < 7u)
        return base[n];
    idx_t block = n / 7;
    idx_t rem   = n % 7;
    seq_t result = (seq_t)block * (seq_t)4 + base[rem];
    return result;
}
