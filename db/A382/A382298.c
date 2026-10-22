/*
 * A382298  Number of states in smallest DFAO computing t(i+n) on input n in
 *          base 2, lsd-first, where t(n) = A010060(n), the Thue-Morse
 *          sequence.
 * https://oeis.org/A382298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the empirical recurrence (unproven) from
 *            A382298: a(0) = 2, a(1) = 3, a(2) = 5, a(3) = 6, a(4) = 7, a(5)
 *            = 8, a(6) = 9, a(7) = 9, a(8) = 9, a(9) = 10; For n ≥ 10, let m
 *            = ⌊log₂ n⌋ and let 2^m ≤ n < 2^(m+1). Put i = n - 2^m. There are
 *            two cases: Case 1 (0 ≤ i < 2^(m-1)): a(n) =...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Required includes for both C and C++ -------------------------------- */
#include <stdint.h>

/* --- Helper: floor of log₂ for an unsigned 64‑bit integer ---------------- */
static inline idx_t floor_log2_uint64(idx_t v)
{
    /* v > 0 is guaranteed by the caller (n ≥ 1 in recursive calls).  */
    idx_t r = 0;
    while (v > 1)
    {
        v >>= 1;
        ++r;
    }
    return r;
}

/* --- Base values for n = 0 … 9 ------------------------------------------ */
static const seq_t base_values[10] = {
    /* 0 */ 2, /* 1 */ 3, /* 2 */ 5, /* 3 */ 6,
    /* 4 */ 7, /* 5 */ 8, /* 6 */ 9, /* 7 */ 9,
    /* 8 */ 9, /* 9 */10
};

/* --- Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 10)
        return base_values[n];

    /* Find m such that 2^m ≤ n < 2^(m+1) */
    idx_t m = floor_log2_uint64(n);
    idx_t pow2_m = (idx_t)1 << m;          /* 2^m  */
    idx_t i = n - pow2_m;                 /* offset from 2^m */

    /* Branch into the two empirical cases */
    if (i < (pow2_m >> 1)) /* Case 1: 0 ≤ i < 2^(m-1) */
    {
        idx_t part = pow2_m >> 2; /* 2^(m-2) */
        idx_t base_n = (pow2_m >> 1) + i; /* 2^(m-1) + i */

        seq_t inc;
        if (i == 0 || i >= part)
            inc = (seq_t)2;
        else if (i == 1 || i == part - 1)
            inc = (seq_t)3;
        else
            inc = (seq_t)4;

        return a(base_n) + inc;
    }
    else /* Case 2: 2^(m-1) ≤ i < 2^m */
    {
        idx_t i2 = i - (pow2_m >> 1);         /* i - 2^(m-1) */
        idx_t base_n = pow2_m + i2;           /* 2^m + (i - 2^(m-1)) */

        seq_t inc = (i2 == ((pow2_m >> 1) - 1)) ? (seq_t)1 : (seq_t)2;
        return a(base_n) + inc;
    }
}
