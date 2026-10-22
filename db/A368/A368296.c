/*
 * A368296  Square array T(n,k), n >= 2, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^(n-j) * floor(j/2).
 * https://oeis.org/A368296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) for the coordinates (r,k) obtained by reading the
 *            square array T(r,k) of non‑negative integers by antidiagonals
 *            downwards, where T(r,k) = ∑_{j=0..r} k^{r-j} · ⌊j/2⌋
 * Range:     u8 n <= 41, u16 n <= 86, u32 n <= 202, u64 n <= 484,
 *            u128 n <= 1209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function below computes the n‑th term of OEIS sequence
    A368296.  It works for any build where seq_t and idx_t are
    unsigned integer types (uint8_t…uint64_t, unsigned __int128,
    or an arbitrary‑precision integer class).  The implementation
    uses only integer arithmetic and has a time complexity of
    O(r) with r ≈ √(2·n), which is well below 2 s for n ≤ 10 000.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /*  a(2) is the first term; return 0 for all smaller indices. */
    if (n < 2U)
        return (seq_t)0;

    /* Convert the sequence index to a zero‑based position. */
    idx_t i   = n - 2;          /* 0‑based index of the element */
    idx_t pos = i + 1;          /* 1‑based index for the diagonal search */

    /* Find the diagonal s containing the element.
       Solve (s‑1)·s/2  >=  pos .
       Use a double sqrt for a good initial guess and correct
       any off‑by‑one errors with a few small integer steps. */
    long double disc = sqrtl((long double)(1 + 8 * (long double)pos));
    idx_t s = (idx_t)((1.0L + disc) / 2.0L);     /*  initial estimate    */

    /* Correct the estimate.  The corrections are at most a few steps. */
    while (((__uint128_t)(s - 1) * s) / 2 < pos)
        ++s;
    while (((__uint128_t)(s - 2) * (s - 1)) / 2 >= pos)
        --s;

    /*  Number of terms before diagonal s (i.e., up to diagonal s‑1). */
    idx_t prev = (idx_t)(((__uint128_t)(s - 2) * (s - 1)) / 2);

    /*  Position inside diagonal s.  Elements of diagonal s are
        listed with row index n ranging from 2 to s, in increasing order. */
    idx_t offset = i - prev;                /* 0 ≤ offset ≤ s‑2  */
    idx_t rn = 2 + offset;                  /*  row index  r = n  */
    idx_t rk = s - rn;                      /*  column index      */

    /*  Compute the value T(rn, rk).
        The formula is a straightforward accumulation of
        powers k^{r-j} multiplied by floor(j/2). */
    seq_t result = (seq_t)0;
    seq_t pow    = (seq_t)1;                /*  k^{r-n}  with n = r   */
    for (idx_t j = rn; ; --j) {
        seq_t v = (seq_t)(j >> 1);          /* floor(j/2)   */
        if (v != 0)
            result += pow * v;
        if (j == 0)
            break;
        pow *= (seq_t)rk;                   /* move to next power    */
    }

    return result;
}
