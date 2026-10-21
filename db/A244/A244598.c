/*
 * A244598  Integers n such that for every k > 0, n*10^k-1 has a divisor in
 *          the set { 11, 73, 101, 137 }.
 * https://oeis.org/A244598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 8, a(n) = a(n-8) + 11111111; initial values:
 *            a(1)=152206, a(2)=1522060, a(3)=4109489, a(4)=4459665,
 *            a(5)=6001522, a(6)=7761557, a(7)=9489041, a(8)=10948904
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base terms for indices 1..8 */
    static const seq_t base[8] = {
        152206ULL,
        1522060ULL,
        4109489ULL,
        4459665ULL,
        6001522ULL,
        7761557ULL,
        9489041ULL,
        10948904ULL
    };

    /* adjust 0‑based offset */
    idx_t idx0 = n - 1;
    idx_t q = idx0 / 8;          /* how many 8‑step jumps from the start */
    idx_t r = idx0 % 8;          /* position within the base block */

    /* result = base[r] + q*11111111 */
    seq_t result = base[r];
    result += ((seq_t)q) * (seq_t)11111111;
    return result;
}
