/*
 * A321371  n + [n*r/s] + [n*t/s], where r = 1, s = cos(2*Pi/5), t =
 *          sec(2*Pi/5).
 * https://oeis.org/A321371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·r/s⌋ + ⌊n·t/s⌋ where r = 1, s = cos(2π/5), t =
 *            sec(2π/5). Using trigonometric identities this simplifies to:
 *            a(n) = 8·n + ⌊√(5·n²)⌋ + ⌊√(20·n²)⌋. The function implements
 *            this formula using only integer arithmetic, avoiding
 *            floating‑point operations
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(√x) for any seq_t value x  (works for built‑in
   integers and arbitrary‑precision types).  Uses binary search with division
   to avoid overflow. */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            ans = 0;
            lo = 1;
            continue;
        }
        /* compare mid with x / mid to avoid mid*mid overflow */
        seq_t div = x / mid;
        if (mid <= div) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                      /* n² */
    seq_t term1 = floor_sqrt_seq(n2 * (seq_t)5);   /* ⌊√(5·n²)⌋ */
    seq_t term2 = floor_sqrt_seq(n2 * (seq_t)20);  /* ⌊√(20·n²)⌋ */
    seq_t res   = (seq_t)8 * nn + term1 + term2;
    return res;
}
