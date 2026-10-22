/*
 * A360259  a(0) = 0, and for any n > 0, let k > 0 be as small as possible and
 *          such that F(2) + ... + F(1+k) >= n (where F(m) denotes A000045(m),
 *          the m-th Fibonacci number); a(n) = k + a(F(2) + ... + F(1+k) - n).
 * https://oeis.org/A360259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0; for n>0 let k = smallest positive integer with
 *            F(2)+...+F(1+k) >= n, then a(n)=k + a( (F(2)+...+F(1+k)) - n )
 * Range:     u8 n <= 70, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fibonacci numbers sufficient for n up to at least 10,000
   (k≤18, so indices up to 21 are needed). */
static seq_t fib[100];
static bool fib_inited = false;

static void init_fib(void)
{
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    for (idx_t i = 2; i < 100; ++i)
        fib[i] = fib[i-1] + fib[i-2];
    fib_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!fib_inited) init_fib();

    idx_t k = 1;
    while (true) {
        /* S(k) = fib[k+3] - 2 */
        seq_t thresh = fib[k+3] - (seq_t)2;
        if (thresh >= n)
            break;
        ++k;
    }

    seq_t diff = fib[k+3] - (seq_t)2 - (seq_t)n;
    return (seq_t)k + a((idx_t)diff);
}
