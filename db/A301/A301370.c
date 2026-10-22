/*
 * A301370  Maximum determinant of an n X n (0,1)-matrix that has exactly 2*n
 *          ones.
 * https://oeis.org/A301370
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 0 if n = 2; otherwise, if n = 3k → 2^k, if n = 3k+1 → 2^k
 *            for k ≤ 2, → 9·2^(k−3) for k ≥ 3, if n = 3k+2 → 3·2^(k−1)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* offset: first term is a(2). Return 0 for n = 2 */
    if (n == 2)
        return 0;

    /* All other n ≥ 3 */
    idx_t k = n / 3;      /* floor(n/3) */
    idx_t r = n % 3;      /* n mod 3 */

    if (r == 0) {                     /* n = 3k */
        return pow_uint(2ULL, k);
    } else if (r == 1) {              /* n = 3k + 1 */
        if (k <= 2) {                 /* first two cases: 2^k */
            return pow_uint(2ULL, k);
        } else {                      /* k ≥ 3: 9 · 2^(k–3) */
            return pow_uint(2ULL, k - 3) * 9;
        }
    } else { /* r == 2, i.e., n = 3k + 2 */
        /* k ≥ 1, so k–1 is safe even for k = 1 */
        return pow_uint(2ULL, k - 1) * 3;
    }
}
