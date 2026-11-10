/*
 * A324106  Multiplicative with a(p^e) = A005940(p^e).
 * https://oeis.org/A324106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} A005940(p^e), where A005940(m) =
 *            f(m-1,1,1), f(n,i,x) = x if n=0, f(n/2,i+1,x) if n even,
 *            f((n-1)/2,i,x*prime(i)) if n odd
 * Range:     u8 n <= 58, u16 n <= 508, u32 n <= 16272, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 101 terms through n = 9971 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A324106_pr[80];
static int A324106_np = 0;

static void A324106_init(void) {
    if (A324106_np) return;
    uint64_t c = 2;
    while (A324106_np < 80) {
        bool ok = true;
        for (int j = 0; j < A324106_np && A324106_pr[j] * A324106_pr[j] <= c; j++)
            if (c % A324106_pr[j] == 0) { ok = false; break; }
        if (ok) A324106_pr[A324106_np++] = c;
        c++;
    }
}

static seq_t A324106_doudna(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)A324106_pr[i - 1];
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    A324106_init();
    seq_t res = 1;
    idx_t m = n;
    for (idx_t d = 2; d <= m / d; d++) {
        if (m % d == 0) {
            idx_t q = 1;
            while (m % d == 0) { m /= d; q *= d; }
            res *= A324106_doudna(q);
        }
    }
    if (m > 1) res *= A324106_doudna(m);
    return res;
}
