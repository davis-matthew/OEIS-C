/*
 * A366596  Repdigit numbers that are divisible by 7.
 * https://oeis.org/A366596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * a(1) = 0 * For n ≥ 2 let r = (n-2) mod 14 and k = (n-2)/14. *
 *            If r ≤ 4: a(n) = 7 * (10^(6k+1+r) - 1) / 9 * If r > 4: a(n) =
 *            (r-4) * (10^(6(k+1)) - 1) / 9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 18, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t x = n - 2;          /* shift index to start at 0 */
    idx_t r = x % 14;         /* remainder in the 14‑term block */
    idx_t k = x / 14;         /* block number */

    seq_t t;
    if (r <= 4) {
        idx_t len = 6 * k + 1 + r;
        t = pow10(len);          /* 10^len */
        t = t - (seq_t)1;
        t = t / (seq_t)9;
        return t * (seq_t)7;
    } else {
        seq_t d = (seq_t)(r - 4);    /* digit 1..9 */
        idx_t len = 6 * (k + 1);
        t = pow10(len);
        t = t - (seq_t)1;
        t = t / (seq_t)9;
        return t * d;
    }
}
