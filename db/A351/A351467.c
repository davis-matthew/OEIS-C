/*
 * A351467  Numbers with integer defect at most 1; m such that A350723(m) <=
 *          1.
 * https://oeis.org/A351467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  a(n) is the nth integer m with integer defect ≤ 1; 
   equivalently, m can be written as 2^p 3^r with p ≤ 10, or as
   2^p (2^q 3^r + 1) 3^s with p + q ≤ 2.  This implementation
   tests each candidate m sequentially. */

#include <stdbool.h>

static bool is_power_of_3(seq_t x)
{
    if (x == 0) return false;
    while (x > 1 && x % 3 == 0) x /= 3;
    return x == 1;
}

static seq_t pow2(int p)
{
    seq_t r = 1;
    for (int i = 0; i < p; ++i) r *= 2;
    return r;
}

static bool satisfies_defect(seq_t m)
{
    /* Type 1: 2^p 3^r, 0 ≤ p ≤ 10 */
    seq_t tmp = m;
    idx_t p2 = 0;
    while (tmp % 2 == 0) { tmp /= 2; ++p2; }
    if (p2 <= 10) {
        if (tmp == 1 || is_power_of_3(tmp))
            return true;
    }

    /* Type 2: 2^p(2^q 3^r + 1)3^s, 0 ≤ p+q ≤ 2 */
    for (int p = 0; p <= 2; ++p) {
        seq_t pow2p = pow2(p);
        if (m % pow2p != 0) continue;
        seq_t m1 = m / pow2p;

        /* Extract power of 3 => m2 = 2^q 3^r + 1 */
        seq_t tmp3 = m1;
        while (tmp3 % 3 == 0) tmp3 /= 3;
        seq_t m2 = tmp3;          /* m2 = 2^q 3^r + 1 */
        if (m2 <= 1) continue;    /* must be > 1 to subtract 1 */

        int qmax = 2 - p;
        for (int q = 0; q <= qmax; ++q) {
            seq_t pow2q = pow2(q);
            seq_t diff = m2 - 1;
            if (diff % pow2q != 0) continue;
            seq_t t = diff / pow2q;   /* t should be 3^r */
            if (is_power_of_3(t))
                return true;
        }
    }

    return false;
}

seq_t a(idx_t n)
{
    seq_t m = 1;
    idx_t count = 0;
    while (true) {
        if (satisfies_defect(m)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
