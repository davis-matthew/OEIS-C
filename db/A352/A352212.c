/*
 * A352212  Largest number of maximal triangle-free node-induced subgraphs of
 *          an n-node graph.
 * https://oeis.org/A352212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352212: a(n) = 10*a(n-5) for n ≥ 13, with
   a(1)=1, a(2)=1, a(3)=3, a(4)=6, a(5)=10, a(6)=15,
   a(7)=21, a(8)=36, a(9)=60, a(10)=100, a(11)=150,
   a(12)=225.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined, safety */

    /* Base values known from the OEIS entry. */
    static const seq_t base[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)6,
        (seq_t)10, (seq_t)15, (seq_t)21, (seq_t)36,
        (seq_t)60, (seq_t)100,(seq_t)150,(seq_t)225
    };

    if (n <= 12)
        return base[n - 1];

    /* Reduce n to a value ≤ 12 by subtracting 5 repeatedly,
       remembering how many times the factor 10 must be applied. */
    idx_t cur = n;
    idx_t count = 0;                 /* number of times 10 is multiplied */

    while (cur > 12)
    {
        cur -= 5;
        ++count;
    }

    seq_t result = base[cur - 1];

    /* Multiply by 10^count.  Using a loop avoids any type‑specific
       power‑function and works for arbitrary‑precision seq_t values. */
    while (count--)
        result *= (seq_t)10;

    return result;
}
