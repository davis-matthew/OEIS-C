/*
 * A317905  Convergence speed of m^^m, where m = A067251(n) and n >= 2. a(n) =
 *          f(m, m) - f(m, m - 1), where f(x, y) corresponds to the maximum
 *          value of k, such that x^^y == x^^(y + 1) (mod 10^k).
 * https://oeis.org/A317905
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   with m = A067251(n) = n + floor((n-1)/9), a(2) = 0 and for n >
 *            2: a(n) = v_5(m^2+1) if m == 2,8 (mod 10) a(n) = v_5(m^2-1) if m
 *            == 4,6 (mod 10) a(n) = v_2(m^2-1) - 1 if m == 5 (mod 10) a(n) =
 *            min(v_2(m^2-1) - 1, v_5(m^2-1)) if m == 1,9 (mod 10) a(n) =
 *            min(v_2(m^2-1) - 1, v_5(m^2+1)) if m == 3,7 (...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 88 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A317905_val(unsigned __int128 x, unsigned p)
{
    int v = 0;
    if (x == 0) return 0;
    while (x % p == 0) { x /= p; v++; }
    return v;
}

seq_t a(idx_t n)
{
    unsigned __int128 m, sq;
    int r, v2, v5;
    if (n <= 2) return (seq_t)0;
    m = (unsigned __int128)n + (unsigned __int128)((n - 1) / 9);
    sq = m * m;
    r = (int)(m % 10);
    switch (r) {
    case 2: case 8:
        return (seq_t)A317905_val(sq + 1, 5);
    case 4: case 6:
        return (seq_t)A317905_val(sq - 1, 5);
    case 5:
        return (seq_t)(A317905_val(sq - 1, 2) - 1);
    case 1: case 9:
        v2 = A317905_val(sq - 1, 2) - 1;
        v5 = A317905_val(sq - 1, 5);
        return (seq_t)(v2 < v5 ? v2 : v5);
    default: /* 3, 7 */
        v2 = A317905_val(sq - 1, 2) - 1;
        v5 = A317905_val(sq + 1, 5);
        return (seq_t)(v2 < v5 ? v2 : v5);
    }
}
