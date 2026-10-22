/*
 * A378880  a(n) = A378879(n) - A083025(n).
 * https://oeis.org/A378880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{p^e || n} e * (1 if p = 2 or p ≡ 3 (mod 4)
 *                            else -1 if p ≡ 1 (mod 4))
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    uint64_t m = n;

    if (m < 2)
        return 0;

    /* handle factor 2 */
    while ((m & 1) == 0) {
        res += 1;          /* p = 2 -> coefficient +1 */
        m >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* p ≡ 3 (mod 4) → +1, p ≡ 1 (mod 4) → -1 */
            if (p % 4 == 3)
                res += exp;
            else
                res -= exp;
        }
    }

    /* remaining prime factor if any */
    if (m > 1) {
        if (m % 4 == 3)
            res += 1;
        else
            res -= 1;
    }

    return res;
}
