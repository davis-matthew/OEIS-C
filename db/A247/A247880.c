/*
 * A247880  For bases b = 2, 3, ..., n, let the base-b expansion of n be
 *          [c_{1,b} c_{2,b} .. c_{r_b,b}], with the most significant "digit"
 *          on the left, 0 <= c_{i,b} < b, and c_{1,b} != 0; then a(n) is the
 *          number whose base-n expansion is the sum of all those expansions,
 *          regarding the c_{i,j} as integers mod n.
 * https://oeis.org/A247880
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{b=2}^{n} (interpret the base‑b representation of n as
 *            a base‑n number)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n ≥ 2 */

    /* maximum number of base‑2 digits for a 64‑bit integer: 64 (plus one for safety) */
    const int MAX_DIGITS = 65;
    unsigned char digits[MAX_DIGITS];

    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;       /* cast once to avoid repeated casts */

    for (idx_t b = 2; b <= n; ++b) {
        idx_t temp = n;
        int len = 0;

        /* extract digits of n in base b (least significant first) */
        do {
            digits[len++] = temp % b;
            temp /= b;
        } while (temp);

        /* interpret those digits as a number in base n */
        seq_t val = 0;
        for (int i = len - 1; i >= 0; --i) {
            val = val * n_seq + (seq_t)digits[i];
        }

        sum += val;
    }

    return sum;
}
