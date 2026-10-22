/*
 * A348354  The base-5 expansion of a(n) is obtained by replacing 1's, 2's,
 *          3's and 4's by 3's, 4's, 1's and 2's, respectively, in the base-5
 *          expansion of n.
 * https://oeis.org/A348354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number whose base‑5 representation is obtained from
 *            the base‑5 representation of n by mapping digits 1→3, 2→4, 3→1,
 *            4→2, while 0 stays unchanged
 * Range:     u8 n <= 124, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles n = 0 immediately. */
    if (n == 0)
        return (seq_t)0;

    /* result accumulates the mapped digits weighted by powers of 5. */
    seq_t res = (seq_t)0;
    seq_t pow = (seq_t)1;            /* 5⁰ initially */

    /* Process digits from least significant to most significant. */
    for (; n > 0; n /= 5) {
        idx_t d = n % 5;              /* current digit in base 5 */
        seq_t mapped;

        /* Map digit according to the permutation. */
        switch (d) {
            case 1: mapped = (seq_t)3; break;
            case 2: mapped = (seq_t)4; break;
            case 3: mapped = (seq_t)1; break;
            case 4: mapped = (seq_t)2; break;
            default: mapped = (seq_t)0;      /* d == 0 */
        }

        /* Add the mapped digit in the correct positional value. */
        res += mapped * pow;

        /* Prepare power for the next digit. */
        pow *= (seq_t)5;
    }

    return res;
}
