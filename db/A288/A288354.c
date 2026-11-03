/*
 * A288354  Strings of 4 digits from 1...9, such that no formula using the
 *          single digits in the given order exists that evaluates to 0.
 * https://oeis.org/A288354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Sequence A288354 computed by brute force enumeration of all *
 *            4‑digit strings from 1…9, keeping those that cannot be evaluated
 *            to * 0 by any arithmetic expression using the digits in the
 *            given order
 * Range:     u8 none, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Validated: full, pass, 1500 terms through n = 1500 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Rational helper, using 64‑bit integers for numerators/denominators. */
static inline long long llgcd(long long a, long long b)
{
    while (b) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a >= 0 ? a : -a;
}

static inline void normalize(long long *num, long long *den)
{
    if (*den < 0) {
        *den = -*den;
        *num = -*num;
    }
    long long g = llgcd(*num < 0 ? -*num : *num, *den);
    if (g > 1) {
        *num /= g;
        *den /= g;
    }
}

static inline bool apply_op(long long aNum, long long aDen,
                            int op, long long bNum, long long bDen,
                            long long *resNum, long long *resDen)
{
    switch (op) {
    case 0: /* + */
        *resNum = aNum * bDen + bNum * aDen;
        *resDen = aDen * bDen;
        normalize(resNum, resDen);
        return true;
    case 1: /* - */
        *resNum = aNum * bDen - bNum * aDen;
        *resDen = aDen * bDen;
        normalize(resNum, resDen);
        return true;
    case 2: /* * */
        *resNum = aNum * bNum;
        *resDen = aDen * bDen;
        normalize(resNum, resDen);
        return true;
    case 3: /* / */
        if (bNum == 0) return false;          /* invalid division */
        *resNum = aNum * bDen;
        *resDen = aDen * bNum;
        normalize(resNum, resDen);
        return true;
    default:
        return false;
    }
}

/* Return true if any expression formed from the 4 digits evaluates to 0. */
static inline bool has_zero_expression(const int digits[4])
{
    long long n0 = digits[0], n1 = digits[1], n2 = digits[2], n3 = digits[3];
    long long d0 = 1, d1 = 1, d2 = 1, d3 = 1;

    for (int op0 = 0; op0 < 4; ++op0)
        for (int op1 = 0; op1 < 4; ++op1)
            for (int op2 = 0; op2 < 4; ++op2) {
                long long t01n, t01d, t02n, t02d, t03n, t03d;
                /* Pattern 1: ((n0 op0 n1) op1 n2) op2 n3 */
                if (!apply_op(n0, d0, op0, n1, d1, &t01n, &t01d)) continue;
                if (!apply_op(t01n, t01d, op1, n2, d2, &t02n, &t02d)) continue;
                if (!apply_op(t02n, t02d, op2, n3, d3, &t03n, &t03d)) continue;
                if (t03n == 0) return true;

                long long t12n, t12d, t0p12n, t0p12d, r2n, r2d;
                /* Pattern 2: (n0 op0 (n1 op1 n2)) op2 n3 */
                if (!apply_op(n1, d1, op1, n2, d2, &t12n, &t12d)) continue;
                if (!apply_op(n0, d0, op0, t12n, t12d, &t0p12n, &t0p12d)) continue;
                if (!apply_op(t0p12n, t0p12d, op2, n3, d3, &r2n, &r2d)) continue;
                if (r2n == 0) return true;

                long long t01a, t01b, t23n2, t23d2, r3n, r3d;
                /* Pattern 3: (n0 op0 n1) op1 (n2 op2 n3) */
                if (!apply_op(n0, d0, op0, n1, d1, &t01a, &t01b)) continue;
                if (!apply_op(n2, d2, op2, n3, d3, &t23n2, &t23d2)) continue;
                if (!apply_op(t01a, t01b, op1, t23n2, t23d2, &r3n, &r3d)) continue;
                if (r3n == 0) return true;

                long long t12a, t12b, t12p3n, t12p3d, t0p12p3n, t0p12p3d;
                /* Pattern 4: n0 op0 ((n1 op1 n2) op2 n3) */
                if (!apply_op(n1, d1, op1, n2, d2, &t12a, &t12b)) continue;
                if (!apply_op(t12a, t12b, op2, n3, d3, &t12p3n, &t12p3d)) continue;
                if (!apply_op(n0, d0, op0, t12p3n, t12p3d, &t0p12p3n, &t0p12p3d)) continue;
                if (t0p12p3n == 0) return true;

                long long t23na, t23nb, t12p23n, t12p23d, t0p12p23n, t0p12p23d;
                /* Pattern 5: n0 op0 (n1 op1 (n2 op2 n3)) */
                if (!apply_op(n2, d2, op2, n3, d3, &t23na, &t23nb)) continue;
                if (!apply_op(n1, d1, op1, t23na, t23nb, &t12p23n, &t12p23d)) continue;
                if (!apply_op(n0, d0, op0, t12p23n, t12p23d, &t0p12p23n, &t0p12p23d)) continue;
                if (t0p12p23n == 0) return true;
            }
    return false;
}

/* --- Main sequence function ------------------------------------------- */
static seq_t seq_cache[1500];
static bool seq_ready = false;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    if (!seq_ready) {
        static const int MAX_COUNT = 1500;
        int d[4];
        sepaml:
        for (int d1 = 1; d1 <= 9; ++d1) {
            for (int d2 = 1; d2 <= 9; ++d2) {
                for (int d3 = 1; d3 <= 9; ++d3) {
                    for (int d4 = 1; d4 <= 9; ++d4) {
                        d[0] = d1; d[1] = d2; d[2] = d3; d[3] = d4;
                        if (!has_zero_expression(d)) {
                            seq_t val = (seq_t)(d1 * 1000 + d2 * 100 + d3 * 10 + d4);
                            seq_cache[seq_len++] = val;
                        }
                    }
                }
            }
        }
        seq_ready = true;
    }

    if (n == 0 || n > seq_len) return 0;   /* out of bounds -> 0 */
    return seq_cache[n - 1];
}
