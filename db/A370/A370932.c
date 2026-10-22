/*
 * A370932  For any number n >= 0 with ternary expansion Sum_{i >= 0} t_i *
 *          3^i, a(n) = Sum_{i >= 0} ((Sum_{j >= 0} (-1)^j * t_{i+j}) mod 3) *
 *          3^i.
 * https://oeis.org/A370932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Σ_{i>=0} ( ( Σ_{j>=0} (-1)^j * t_{i+j} ) mod 3 ) * 3^i
 *            where t_i are the base‑3 digits of n (t_0 least significant)
 * Range:     u8 n <= 242, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Extract the base‑3 digits of n (least significant first). */
    unsigned char digits[70];          /* enough for 64‑bit n (log_3 2^64 < 40). */
    size_t k = 0;
    idx_t m = n;
    if (m == 0) {
        digits[0] = 0;
        k = 1;
    } else {
        while (m > 0) {
            digits[k++] = (unsigned char)(m % (idx_t)3);
            m /= (idx_t)3;
        }
    }

    /* Compute the alternating suffix sums modulo 3. */
    unsigned char suffix[70];
    int alt = 0;             /* suffix value in the range 0..2 */
    for (size_t i = k; i-- > 0; ) {
        int t = (int)digits[i] - alt;
        t %= 3;
        if (t < 0) t += 3;
        alt = t;
        suffix[i] = (unsigned char)alt;
    }

    /* Build the result from its base‑3 digits. */
    seq_t result = 0;
    seq_t pow3 = 1;          /* 3^i */
    for (size_t i = 0; i < k; ++i) {
        result += (seq_t)suffix[i] * pow3;
        pow3 *= (seq_t)3;
    }
    return result;
}
