/*
 * A361947  If the rightmost nonzero digit in the base-4 expansion of n is the
 *          digit 1, then replace 2's by 3's and vice versa; if it is the
 *          digit 2, then replace 1's by 3's and vice versa; if it is the
 *          digit 3, then replace 1's by 2's and vice versa; a(0) = 0.
 * https://oeis.org/A361947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is obtained by mapping the base‑4 digits of n according *
 *            to the rightmost nonzero digit: * if that digit is 1: swap 2↔3 *
 *            if that digit is 2: swap 1↔3 * if that digit is 3: swap 1↔2 *
 *            a(0)=0. * This implements A361947
 * Range:     u8 n <= 255, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the rightmost non‑zero base‑4 digit. */
    idx_t tmp = n;
    unsigned int rightmost = 0;
    while (tmp > 0) {
        unsigned int d = tmp % 4;
        tmp /= 4;
        if (d != 0) {
            rightmost = d;
            break;          /* first non‑zero digit from the right */
        }
    }

    /* Generate the mapped number. */
    tmp = n;
    seq_t result = 0;
    seq_t power  = 1;

    while (tmp > 0) {
        unsigned int d = tmp % 4;
        tmp /= 4;

        unsigned int mapped;
        switch (rightmost) {
            case 1: mapped = (d == 2 ? 3 : (d == 3 ? 2 : d)); break;
            case 2: mapped = (d == 1 ? 3 : (d == 3 ? 1 : d)); break;
            case 3: mapped = (d == 1 ? 2 : (d == 2 ? 1 : d)); break;
            default: mapped = d; /* should never occur */
        }

        result += (seq_t)mapped * power;
        power *= 4;
    }

    return result;
}
