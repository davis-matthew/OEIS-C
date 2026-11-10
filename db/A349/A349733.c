/*
 * A349733  Composite numbers that are missing from A349194.
 * https://oeis.org/A349733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th composite k that is not a product of prefix digit
 *            sums of any m (search over k, DFS over chains s_1<=...<=s_L,
 *            s_1<=9, s_{i+1}-s_i in 0..9)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* can k be written as a chain product ending with last element s */
static bool A349733_f(unsigned long long k, unsigned long long s)
{
    if (s == 0 || k % s != 0) return false;
    unsigned long long r = k / s;
    if (r == 1) return s <= 10;
    unsigned long long lo = (s > 9) ? s - 9 : 1;
    if (lo < 2) lo = 2;
    for (unsigned long long t = lo; t <= s; t++) {
        if (r % t == 0 && A349733_f(r, t)) return true;
    }
    return false;
}

static bool A349733_inimage(unsigned long long k)
{
    if (k == 1) return true;
    for (unsigned long long s = 1; s <= k; s++) {
        if (k % s == 0 && A349733_f(k, s)) return true;
    }
    return false;
}

static bool A349733_prime(unsigned long long k)
{
    if (k < 2) return false;
    for (unsigned long long d = 2; d * d <= k; d++)
        if (k % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    unsigned long long k = 3;
    idx_t cnt = 0;
    while (cnt < n) {
        k++;
        if (!A349733_prime(k) && !A349733_inimage(k))
            cnt++;
    }
    return (seq_t)k;
}
