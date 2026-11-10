/*
 * A340032  Irregular triangle read by rows T(n,k) in which row n lists n
 *          blocks, where the m-th block consists of A000041(n-m) copies of
 *          the row m of triangle A127093, with 1 <= m <= n.
 * https://oeis.org/A340032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Row n of the triangle is the concatenation, for m = 1..n, of
 *            A000041(n-m) copies of row m of A127093 (row m = [k if k divides
 *            m else 0 for k = 1..m]). Row n has Sum_{m=1..n} m*A000041(n-m)
 *            terms
 * Range:     u8 n <= 11552, u16 n <= 11552, u32 n <= 11552, u64 n <= 11552,
 *            u128 n <= 11552, big any n
 * Validated: standard, pass, 112 terms through n = 9726 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A340032_PMAX 400

typedef unsigned __int128 A340032_u128;

static A340032_u128 A340032_part[A340032_PMAX + 1];
static int A340032_init = 0;

static void A340032_setup(void)
{
    size_t i, j;
    if (A340032_init) return;
    A340032_part[0] = 1;
    for (i = 1; i <= A340032_PMAX; i++) A340032_part[i] = 0;
    for (i = 1; i <= A340032_PMAX; i++)
        for (j = i; j <= A340032_PMAX; j++)
            A340032_part[j] += A340032_part[j - i];
    A340032_init = 1;
}

seq_t a(idx_t n)
{
    A340032_u128 target, cum = 0;
    size_t row, m;
    if (n == 0) return (seq_t)0;
    A340032_setup();
    target = (A340032_u128)n;
    for (row = 1; row <= A340032_PMAX; row++) {
        A340032_u128 len = 0;
        for (m = 1; m <= row; m++)
            len += A340032_part[row - m] * (A340032_u128)m;
        if (cum + len >= target) {
            A340032_u128 off = target - cum - 1;
            for (m = 1; m <= row; m++) {
                A340032_u128 sz = A340032_part[row - m] * (A340032_u128)m;
                if (off < sz) {
                    size_t k = (size_t)(off % (A340032_u128)m) + 1;
                    return (m % k == 0) ? (seq_t)k : (seq_t)0;
                }
                off -= sz;
            }
            return (seq_t)0;
        }
        cum += len;
    }
    return (seq_t)0;
}
