/*
 * A361946  If the base-4 expansion of n starts with the digit 1, then replace
 *          2's by 3's and vice versa; if it starts with the digit 2, then
 *          replace 1's by 3's and vice versa; if it starts with the digit 3,
 *          then replace 1's by 2's and vice versa; a(0) = 0.
 * https://oeis.org/A361946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) transforms the base‑4 expansion of n by swapping two digits
 *            depending on the leading digit: (1 → swap 2/3, 2 → swap 1/3, 3 →
 *            swap 1/2); a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find most significant base‑4 digit */
    idx_t msd = n;
    while (msd >= 4) msd /= 4;  /* msd ∈ {1,2,3} for n>0 */

    seq_t result = 0;
    seq_t mult   = 1;
    idx_t temp   = n;

    while (temp > 0) {
        idx_t d = temp % 4;
        temp   /= 4;

        idx_t dprime;
        switch (msd) {
            case 1:  /* swap 2 ↔ 3 */
                if (d == 2) dprime = 3;
                else if (d == 3) dprime = 2;
                else dprime = d;
                break;
            case 2:  /* swap 1 ↔ 3 */
                if (d == 1) dprime = 3;
                else if (d == 3) dprime = 1;
                else dprime = d;
                break;
            case 3:  /* swap 1 ↔ 2 */
                if (d == 1) dprime = 2;
                else if (d == 2) dprime = 1;
                else dprime = d;
                break;
            default: dprime = d;  /* should not occur for n>0 */
        }

        result += (seq_t)dprime * mult;
        mult <<= 2;  /* mult *= 4 */
    }

    return result;
}
