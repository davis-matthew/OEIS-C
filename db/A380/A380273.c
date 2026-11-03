/*
 * A380273  Irregular table T(n, k), n >= 0, k = 1..A380272(n), read by rows;
 *          the n-th row lists the integers m in 0..n such that the
 *          nonadjacent forms for m-n and m can be added without carries.
 * https://oeis.org/A380273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3621, u16 n <= 10524, u32 n <= 10524, u64 n <= 10524,
 *            u128 n <= 10524, big any n
 * Validated: standard, pass, 97 terms through n = 9839 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A380273 sequence term a(n) by scanning the irregular
   table T(n,k).  For each row n it iterates over m = 0..n and tests whether
   (n–m) and m have nonadjacent forms that can be added without carries.
   The first m found at the global index n is returned as seq_t. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Compute the Non‑Adjacency Form (NAF) digits of a non‑negative integer x.
   Digits are stored least‑significant first in `digits`.  `len` is set to
   the number of non‑zero digits (len==0 for x==0). */
static inline void naf_compute(uint64_t x, int *digits, size_t *len)
{
    *len = 0;
    while (x != 0) {
        if (x & 1ULL) {
            /* x is odd */
            uint64_t r = x & 3ULL;          /* x mod 4 */
            int digit = (r == 1ULL) ? 1 : -1;  /* r==3 gives -1 */
            digits[(*len)++] = digit;
            x = (x - (uint64_t)digit) >> 1;
        } else {
            digits[(*len)++] = 0;
            x >>= 1;
        }
    }
    /* For x==0 we keep len==0, meaning all higher digits are zero. */
}

/* Check whether the NAFs of n-m and m can be added without carries. */
static inline bool no_carry_add(uint64_t n, uint64_t m)
{
    int nafn[64], nafm[64];
    size_t len_n = 0, len_m = 0;

    /* Only need NAF of n-m and m (both non‑negative as m<=n). */
    naf_compute(n - m, nafn, &len_n);
    naf_compute(m, nafm, &len_m);

    int prev_sum = 0;
    size_t i = 0;
    while (i < len_n || i < len_m) {
        int sum = (i < len_n ? nafn[i] : 0) + (i < len_m ? nafm[i] : 0);
        if (sum > 1 || sum < -1) return false;        /* abs(sum) <= 1 */
        if (sum != 0 && prev_sum != 0) return false;  /* adjacent non‑zeros */
        prev_sum = sum;
        ++i;
    }
    return true;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    uint64_t idx = 0;      /* current global index in the flattened table */
    for (uint64_t row = 0; ; ++row) {
        for (uint64_t m = 0; m <= row; ++m) {
            if (no_carry_add(row, m)) {
                if (idx == n) {
                    return (seq_t)m;
                }
                ++idx;
            }
        }
    }
}
