/*
 * A246080  Paradigm shift sequence for (0,2) production scheme with
 *          replacement.
 * https://oeis.org/A246080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑6) for n ≥ 10 Base values: a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=8, a(9)=10 Sequence is
 *            defined for n ≥ 1 (values for n < 1 are undefined)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 117, u64 n <= 238,
 *            u128 n <= 480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 1..9 */
    static const seq_t base[9] = {
        (seq_t)1U, (seq_t)2U, (seq_t)3U, (seq_t)4U,
        (seq_t)5U, (seq_t)6U, (seq_t)7U, (seq_t)8U,
        (seq_t)10U
    };

    if (n == 0)                /* undefined by offset – return 0 */
        return (seq_t)0U;

    /* reduce n to the range 1..9 by subtracting multiples of 6 */
    idx_t m = n;
    while (m > 9U)
        m -= 6U;

    /* the exponent of 3 is the number of subtractions performed */
    idx_t k = (n - m) / 6U;

    seq_t val = base[m - 1U];  /* this is a(m) for 1 ≤ m ≤ 9 */

    /* multiply by 3^k */
    while (k > 0U) {
        val *= (seq_t)3U;
        --k;
    }

    return val;
}
