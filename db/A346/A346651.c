/*
 * A346651  a(n) is the number of divisors of A139245(n) ending with 2.
 * https://oeis.org/A346651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of A139245(n) = 20*n - 16 with d ==
 *            2 (mod 10); computed by factoring 20n-16 and a DP over divisor
 *            residues mod 10
 * Range:     u8 n <= 13, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A139245_a(idx_t n)
{
    return ((seq_t)n * (seq_t)20) - (seq_t)16;
}

static void A346651_apply(seq_t cnt[10], int pr, int e)
{
    seq_t nw[10];
    for (int r = 0; r < 10; r++) nw[r] = 0;
    int pw = 1;
    for (int j = 0; j <= e; j++) {
        for (int r = 0; r < 10; r++) {
            nw[(r * pw) % 10] += cnt[r];
        }
        pw = (pw * pr) % 10;
    }
    for (int r = 0; r < 10; r++) cnt[r] = nw[r];
}

seq_t a(idx_t n)
{
    seq_t m = A139245_a(n);
    seq_t cnt[10];
    for (int r = 0; r < 10; r++) cnt[r] = 0;
    cnt[1] = 1;
    seq_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            A346651_apply(cnt, (int)(p % 10), e);
        }
        if (p == 2) p = 3; else p += 2;
    }
    if (m > 1) {
        A346651_apply(cnt, (int)(m % 10), 1);
    }
    return cnt[2];
}
