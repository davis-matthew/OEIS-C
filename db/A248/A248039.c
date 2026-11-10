/*
 * A248039  Numbers n such that n*A007954(n) contains the same distinct digits
 *          as n.
 * https://oeis.org/A248039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 0 such that the set of decimal digits of
 *            k*A007954(k) equals that of k; computed by a search (a(1)=0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A007954_a(seq_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t result = 1;
    seq_t temp = n;
    while (temp > 0) {
        result *= (seq_t)(temp % 10);
        temp /= 10;
    }
    return result;
}

static unsigned A248039_mask(seq_t x)
{
    unsigned m = 0;
    if (x == 0) {
        return 1u;
    }
    while (x > 0) {
        m |= 1u << (unsigned)(int)(x % 10);
        x /= 10;
    }
    return m;
}

static bool A248039_ok(seq_t k)
{
    seq_t p = A007954_a(k);
    return A248039_mask(k) == A248039_mask(k * p);
}

seq_t a(idx_t n)
{
    static idx_t cnt = 0;      /* index of last found term */
    static seq_t last = 0;
    static bool init = false;

    if (!init || n < cnt) {
        init = true;
        cnt = 1;
        last = 0;              /* a(1) = 0 */
    }
    while (cnt < n) {
        seq_t k = last + 1;
        while (!A248039_ok(k)) {
            k++;
        }
        last = k;
        cnt++;
    }
    return last;
}
