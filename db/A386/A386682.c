/*
 * A386682  Numbers k such that sum of digits of k equals sum of digits of
 *          1/k.
 * https://oeis.org/A386682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=1, a(2)=8. For e ≥ 1 the sequence repeats the block [10^e,
 *            125·10^(e-2), 8·10^e]. The block for e=1 contains only the first
 *            two terms. This produces 1, 8, 10, 80, 100, 125, 800, 1000,
 *            1250, 8000, …
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    /* Remaining terms start at n = 3 */
    idx_t remaining = n - 3;          /* 0‑based count after the 10¹ term */

    /* Block e = 1 has only two terms */
    if (remaining == 0)                 /* 10¹ */
        return pow_uint(10, 1);
    if (remaining == 1)                 /* 8·10¹ */
        return seq_mul((seq_t)8, pow_uint(10, 1));

    /* From here on we are in full 3‑term blocks for e ≥ 2 */
    remaining -= 2;                    /* skip the two terms of e = 1 block */

    idx_t block = remaining / 3;       /* which e‑block (0‑based) */
    idx_t e     = 2 + block;           /* the exponent of 10 in this block */
    idx_t pos   = remaining % 3;       /* 0 → 10^e, 1 → 125·10^(e‑2), 2 → 8·10^e */

    seq_t pow10  = pow_uint(10, e);
    seq_t pow10_e_minus_2 = pow_uint(10, e - 2);

    switch (pos) {
        case 0: return pow10;
        case 1: return seq_mul((seq_t)125, pow10_e_minus_2);
        case 2: return seq_mul((seq_t)8, pow10);
    }
    return 0; /* unreachable */
}
