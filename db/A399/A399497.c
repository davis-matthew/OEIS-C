/*
 * A399497  H_b(c, d) where b >= c >= d >= 0 ordered by b then c then d, where
 *          H_i is the i-th hyperoperation.
 * https://oeis.org/A399497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   a(n) = H_{A056556(n)}(A056557(n), A056558(n))
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (See problem description for A399497) */
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Hyperoperation H_k(a, b) for k >= 0.
 * Uses seq_t for operands and returns seq_t.
 * Recurrence: H_0(a, b) = b+1
 *             H_k(a, 0) = 1 for k >= 3,
 *                          0 for k == 2,
 *                          a for k == 1,
 *                          b+1 for k == 0
 *             H_k(a, b) = a + b        (k == 1)
 *             H_k(a, b) = a * b        (k == 2)
 *             H_k(a, b) = a^b          (k == 3)
 *             H_k(a, b) = H_{k-1}(a, H_k(a, b-1))   (k >= 4)
 */
static seq_t hyperop(int level, seq_t a, seq_t b)
{
    /* 0‑operand case ----------------------------------------------------- */
    if (b == 0) {
        switch (level) {
        case 0: return 1;          /* H_0(a,0) = 1 */
        case 1: return a;          /* H_1(a,0) = a */
        case 2: return 0;          /* H_2(a,0) = 0 */
        default: return 1;         /* H_k(a,0) = 1 for k >= 3 */
        }
    }

    /* level 0 ------------------------------------------------------------- */
    if (level == 0) return b + 1;          /* H_0(a,b) = b+1 */

    /* levels 1 … 3 ------------------------------------------------------- */
    if (level == 1) return a + b;          /* H_1(a,b) = a+b */
    if (level == 2) return a * b;          /* H_2(a,b) = a*b */
    if (level == 3) return big_pow(a, b);  /* H_3(a,b) = a^b */

    /* level >= 4 --------------------------------------------------------- */
    seq_t prev = hyperop(level, a, b - 1);
    return hyperop(level - 1, a, prev);
}

/* ------------------------------------------------------------------------- */
/*   a(n) – sequence A399497, return H_b(c,d) with b>=c>=d>=0,  */
seq_t a(idx_t n)
{
    /* Find outermost index b by subtracting triangular block sizes. */
    idx_t remaining = n;
    idx_t b = 0;
    while (1) {
        /* Number of triples with given b */
        idx_t count_b = (b + 1) * (b + 2) / 2;
        if (remaining < count_b) break;
        remaining -= count_b;
        b++;
    }

    /* Find inner index c.  For fixed b, there are c+1 values of d per c. */
    idx_t c = 0;
    while (1) {
        idx_t count_c = c + 1;
        if (remaining < count_c) break;
        remaining -= count_c;
        c++;
    }

    /* Remaining offset is d. */
    idx_t d = remaining;

    /* Convert to seq_t and compute hyperoperation. */
    return hyperop((int)b, (seq_t)c, (seq_t)d);
}
