/*
 * A398920  Number of solutions of the Diophantine inequality a*b+b*c+c*d <=
 *          n, with a,b,c >= 1.
 * https://oeis.org/A398920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = #positive integer solutions of a*b + b*c + c*d ≤ n
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (a,b,c,d ≥ 1).                                                     */
/* The algorithm loops over a and b, determines the admissible range   */
/* of c and finally sums floor(T/c) for T = n - a*b.  Complexity O(n)  */

#include <stdint.h>   /* for uint64_t types used by the autogeneration */

/* helper: sum_{c=1..M} floor(T/c)                                           */
static inline seq_t sum_floor_div(idx_t T, idx_t M)
{
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= M)
    {
        idx_t q = T / i;                 /* floor(T/i)   */
        if (q == 0)                      /* should not happen for M ≥ 1 */
            break;
        idx_t next = T / q;              /* last c with same quotient */
        if (next > M)
            next = M;
        seq_t cnt = (seq_t)(next - i + 1); /* number of terms with this q */
        sum += cnt * (seq_t)q;
        i = next + 1;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t a = 1; a <= n; ++a)
    {
        /* for fixed a, b must satisfy b*(a+1) ≤ n-1 to allow M≥1      */
        idx_t maxb = (n == 0) ? 0 : (n - 1) / (a + 1);
        for (idx_t b = 1; b <= maxb; ++b)
        {
            idx_t T = n - a * b;        /* n - a*b   */
            idx_t denom = b + 1;        /* factor of c in inequality */
            idx_t M = T / denom;        /* maximal c  (≥1 by construction) */

            seq_t sumfloor = sum_floor_div(T, M);
            seq_t contrib   = sumfloor - (seq_t)b * (seq_t)M;
            total += contrib;
        }
    }

    return total;
}
