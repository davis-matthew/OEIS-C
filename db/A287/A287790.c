/*
 * A287790  {0->001, 1->110}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th bit of the 0→001, 1→110 transform of the infinite
   Fibonacci word A003849.  The algorithm finds the original Fibonacci
   word bit at position p = ceil(n/3) and then applies the mapping
   (r == 3) ? 1–bit : bit, where r = n – 3·(p – 1).  */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t, necessary for some compilers */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1.  For n = 0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Position in the original Fibonacci word and offset inside the
       3‑bit replacement.  All arithmetic is unsigned. */
    idx_t p = (n + 2) / 3;                     /* ceil(n/3)  */
    idx_t r = n - (p - 1) * 3;                 /* 1 ≤ r ≤ 3 */

    /* Build Fibonacci word lengths |w_k| until we find the smallest
       k with |w_k| ≥ p.  The array is large enough for any 64‑bit
       index; we stop when the next length would exceed p. */
    idx_t len[100];
    len[0] = 1;   /* |w_0| = 1 (symbol “0”) */
    len[1] = 2;   /* |w_1| = 2 (string “01”) */
    size_t k = 1;
    while (len[k] < p)
    {
        ++k;
        __uint128_t next = ( __uint128_t)len[k-1] + ( __uint128_t)len[k-2];
        if (next > (__uint128_t)p)
            len[k] = p;              /* saturate to stop the loop */
        else
            len[k] = (idx_t)next;
    }

    /* Descend the decomposition w_k = w_{k-1} w_{k-2} to locate
       the single symbol at position p. */
    idx_t pos = p;
    while (k >= 2)
    {
        if (pos <= len[k-1])
        {
            k--;
        }
        else
        {
            pos -= len[k-1];
            k -= 2;
        }
    }

    /* Determine the Fibonacci word bit at position p. */
    int fib_bit;
    if (k == 0)
    {
        fib_bit = 0;                      /* w_0 = "0"   */
    }
    else            /* k == 1, w_1 = "01" */
    {
        fib_bit = (pos == 1) ? 0 : 1;
    }

    /* Apply the 0→001, 1→110 mapping.  For offset 3 the bit flips. */
    int result = (r == 3) ? (1 - fib_bit) : fib_bit;
    return (seq_t)result;
}
