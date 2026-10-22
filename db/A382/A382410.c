/*
 * A382410  Area of the unique primitive Pythagorean triple (a,b,c) such that
 *          (a-b+c)/2 is A000032(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) * (Lucas(n)-1) * (2*Lucas(n)-1) where
 *            Lucas(0)=2, Lucas(1)=1, Lucas(n)=Lucas(n-1)+Lucas(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of Lucas numbers, dynamically grown */
    static seq_t *lucas = NULL;
    static idx_t cap = 0;

    /* initialise with first two Lucas numbers */
    if (lucas == NULL) {
        lucas = (seq_t *)malloc(2 * sizeof(seq_t));
        if (lucas == NULL) {
            /* memory failure – abort */
            abort();
        }
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        cap = 2;
    }

    /* grow cache if needed */
    if (n >= cap) {
        idx_t newcap = n + 1;  /* one more than n */
        seq_t *t = (seq_t *)realloc(lucas, newcap * sizeof(seq_t));
        if (t == NULL) {
            /* memory failure – abort */
            abort();
        }
        lucas = t;
        /* compute new Lucas numbers */
        for (idx_t i = cap; i <= n; ++i) {
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
        cap = newcap;
    }

    /* compute the sequence value using the formula */
    seq_t L = lucas[n];
    seq_t Lm1 = L - (seq_t)1;        /* L - 1 */
    seq_t Lx2m1 = (L << 1) - (seq_t)1; /* 2*L - 1 */
    seq_t res = L * Lm1;
    res *= Lx2m1;
    return res;
}
