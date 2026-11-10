/*
 * A330129  a(n) is the last term of the analogous sequence to A121805, but
 *          with initial term n, or -1 if that sequence is infinite.
 * https://oeis.org/A330129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = last term of the comma-sequence recurrence started at n
 *            (a(1)=n, then a(k+1)=a(k)+10*(a(k) mod 10)+y with y the smallest
 *            digit equal to the leading digit of the result), or -1 if
 *            infinite; computed by simulation with period-10 fast-forward
 *            inside each leading-digit range
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 39, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 9941 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t A330129_msd(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)n;
    long long iters = 0;
    for (;;) {
        if (++iters > 2000000LL) return (seq_t)0;
        /* pow = largest power of 10 <= prev */
        seq_t pw = 1;
        int digits = 1;
        while (pw <= prev / 10) { pw *= 10; digits++; }
        if (digits > 300) return (seq_t)0 - (seq_t)1; /* infinite */
        seq_t m = prev / pw;
        seq_t B = (m + 1) * pw;
        bool bok = (B > pw) && (B / (m + 1) == pw);
        if (bok) {
            int mi = (int)(long long)m;
            int x0 = (int)(long long)(prev % 10);
            int x = x0;
            seq_t C = 0;
            do {
                C += (seq_t)(10 * x + mi);
                x = (x + mi) % 10;
            } while (x != x0);
            if (B - 1 - prev >= C) {
                seq_t j = (B - 1 - prev) / C;
                prev += j * C;
            }
        }
        /* single step */
        seq_t x = prev % 10;
        seq_t base = prev + x * 10;
        if (base < prev) return (seq_t)0;
        bool found = false;
        seq_t val = 0;
        for (int d = 0; d <= 9; ++d) {
            seq_t cand = base + (seq_t)d;
            if (cand < base) return (seq_t)0;
            if (A330129_msd(cand) == (seq_t)d) {
                val = cand;
                found = true;
                break;
            }
        }
        if (!found) return prev;
        prev = val;
    }
}
