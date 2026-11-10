/*
 * A296083  a(1) = 0; for n > 1, a(n) = A039653(n) /
 *          gcd(A039653(n),A032741(n)).
 * https://oeis.org/A296083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = A039653(n) / gcd(A039653(n),
 *            A032741(n)), where A039653(n) = sigma(n)-1 and A032741(n) =
 *            tau(n)-1
 * Range:     u8 n <= 107, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9796 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A296083_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i == j) {
                sum += (seq_t)i;
                cnt += 1;
            } else {
                sum += (seq_t)i + (seq_t)j;
                cnt += 2;
            }
        }
    }
    sum -= 1;
    cnt -= 1;
    seq_t g = A296083_gcd(sum, cnt);
    return sum / g;
}
