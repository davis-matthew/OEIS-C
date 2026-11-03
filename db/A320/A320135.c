/*
 * A320135  Number of integer solutions to a^2 + b^2 + 2*c^2 + 13*d^2 = n.
 * https://oeis.org/A320135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of integer solutions to a^2 + b^2 + 2c^2
 *            + 13d^2 = n
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8701 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Returns the number of integer pairs (x,y) with x^2 + y^2 = m. */
static seq_t r2_squares(idx_t m)
{
    if (m == 0)
        return (seq_t)1;

    idx_t tmp = m;
    bool ok   = true;
    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i * i <= tmp; ++i) {
        if (tmp % i) continue;
        idx_t e = 0;
        while (tmp % i == 0) {
            ++e;
            tmp /= i;
        }
        /* Prime p == 3 mod 4 with odd exponent forces r2=0 */
        if ((i & 3) == 3) {
            if ((e & 1) != 0) { ok = false; break; }
        }
        /* Prime p == 1 mod 4 contributes a factor (e+1) */
        else if ((i & 3) == 1) {
            prod = prod * (seq_t)(e + 1);
        }
        /* p == 2 contributes nothing extra */
    }
    if (ok && tmp > 1) {          /* remaining prime > sqrt(tmp) */
        if ((tmp & 3) == 3) {
            ok = false; /* odd exponent 1 */
        } else if ((tmp & 3) == 1) {
            prod = prod * (seq_t)2;   /* e = 1 -> e+1 = 2 */
        }
        /* tmp==2: ignore */
    }
    return ok ? prod * (seq_t)4 : (seq_t)0;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t d = 0; ; ++d) {
        idx_t d2 = d * d;
        idx_t term13 = 13 * d2;
        if (term13 > n)
            break;
        seq_t dmult = (d == 0 ? (seq_t)1 : (seq_t)2);

        idx_t rem1 = n - term13;
        for (idx_t c = 0; ; ++c) {
            idx_t c2 = c * c;
            idx_t term2 = 2 * c2;
            if (term2 > rem1)
                break;
            seq_t cmult = (c == 0 ? (seq_t)1 : (seq_t)2);

            idx_t rem2 = rem1 - term2;
            seq_t r = r2_squares(rem2);
            total += r * (dmult * cmult);
        }
    }
    return total;
}
