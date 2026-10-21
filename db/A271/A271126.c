/*
 * A271126  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 289", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (117*4^(n-1) - 76*3^(n-4) - 171*2^(n-2) - 432) / 9 for n
 *            > 5 * initial terms: a(0)=1, a(1)=4, a(2)=32, a(3)=164,
 *            a(4)=732, a(5)=3108
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only used for consistency with the test harness */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly, where the closed form is
       not defined or would involve negative exponents. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)32;
    if (n == 3) return (seq_t)164;
    if (n == 4) return (seq_t)732;
    if (n == 5) return (seq_t)3108;

    /* For n > 5, use the conjectured closed formula. */
    seq_t pow4 = pow_seq((seq_t)4, n - 1);   /* 4^(n-1) */
    seq_t pow3 = pow_seq((seq_t)3, n - 4);   /* 3^(n-4) */
    seq_t pow2 = pow_seq((seq_t)2, n - 2);   /* 2^(n-2) */

    seq_t term1 = pow4 * (seq_t)117;   /* 117 * 4^(n-1) */
    seq_t term2 = pow3 * (seq_t)76;    /* 76  * 3^(n-4) */
    seq_t term3 = pow2 * (seq_t)171;   /* 171 * 2^(n-2) */

    seq_t numerator = term1;
    numerator -= term2;
    numerator -= term3;
    numerator -= (seq_t)432;

    return numerator / (seq_t)9;
}
