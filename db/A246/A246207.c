/*
 * A246207  Permutation of nonnegative integers: a(0) = 0, a(1) = 1, a(2n) =
 *          A117968(a(n)), a(2n+1) = A117967(1+a(n)).
 * https://oeis.org/A246207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0 a(1) = 1 a(2n) = A117968(a(n)) a(2n+1) = A117967(1 +
 *            a(n)) A117968 and A117967 are the two complementary permutations
 *            obtained by mapping the balanced‑ternary digits of an integer:
 *            balance digit -1 → 1 , 0 → 0 , 1 → 2 (A117968) balance digit -1
 *            → 2 , 0 → 0 , 1 → 1 (A117967) The function...
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 96 terms through n = 7392 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Apply A117968 (flag = true) or A117967 (flag = false) to x. */
static seq_t perm(seq_t x, bool useA117968)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    seq_t res   = (seq_t)0;
    seq_t pow3  = (seq_t)1;          /* 3^position */

    while (x > (seq_t)0) {
        seq_t r = x % (seq_t)3;     /* 0,1,2 */
        x     /= (seq_t)3;

        seq_t m;                    /* mapped base‑3 digit */
        if (r == (seq_t)2) {        /* digit -1 with carry */
            x += (seq_t)1;
            m = useA117968 ? (seq_t)1 : (seq_t)2;
        } else {                    /* r is 0 or 1 */
            if (useA117968)
                m = (r == (seq_t)1) ? (seq_t)2 : (seq_t)0;  /* 1→2, 0→0 */
            else
                m = (r == (seq_t)1) ? (seq_t)1 : (seq_t)0;  /* 1→1, 0→0 */
        }

        res  += m * pow3;
        pow3 *= (seq_t)3;
    }

    return res;
}

/* Main sequence function.  idx_t is an unsigned integer type. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;

    if ((n & (idx_t)1) == (idx_t)0)          /* even index */
        return perm(a(n >> 1), true);       /* A117968(a(n/2)) */
    else                                     /* odd index */
        return perm((seq_t)1 + a(n >> 1), false); /* A117967(1 + a((n-1)/2)) */
}
