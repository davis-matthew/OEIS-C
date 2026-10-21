/*
 * A257761  Positive integers whose square is the sum of 23 consecutive
 *          squares.
 * https://oeis.org/A257761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-2) - a(n-4) Initial values: a(1)=92, a(2)=138,
 *            a(3)=4278, a(4)=6532
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is acceptable for n>0 only. For n == 0 a dummy 0 is returned. */

#include <stdint.h> /* for uint64_t if needed by the build */

/* Cache the computed terms.  The array is 1‑based: cache[1] = a(1). */
static seq_t cache[20001];
static idx_t cached_last = 0;

/* Compute the sequence up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= cached_last)
        return;

    for (idx_t i = cached_last + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[1] = (seq_t)92;
        else if (i == 2)
            cache[2] = (seq_t)138;
        else if (i == 3)
            cache[3] = (seq_t)4278;
        else if (i == 4)
            cache[4] = (seq_t)6532;
        else
            cache[i] = ((seq_t)48 * cache[i - 2]) - cache[i - 4];
    }
    cached_last = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n<1; harmless zero */

    compute_up_to(n);
    return cache[n];
}
