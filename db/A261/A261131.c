/*
 * A261131  Number of ways to write n as the sum of 3 positive palindromes.
 * https://oeis.org/A261131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of unordered triples of positive palindromes
 *            summing to n, computed by brute‑force enumeration
 * Range:     u8 n <= 916, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether a positive integer is a decimal palindrome. */
static bool is_pal(seq_t x)
{
    if (x == 0) return false;
    seq_t rev = 0;
    seq_t orig = x;
    while (x > 0)
    {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev == orig;
}

/* a(idx_t n) as required. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* Generate all palindromes <= n.  The count is < 200 for n <= 10000. */
    const int MAXPAL = 5000;
    seq_t pal[MAXPAL];
    idx_t pcount = 0;

    for (idx_t v = 1; v <= n && pcount < MAXPAL; ++v)
    {
        if (is_pal((seq_t)v))
            pal[pcount++] = (seq_t)v;
    }

    seq_t result = 0;

    for (idx_t i = 0; i < pcount; ++i)
    {
        seq_t p1 = pal[i];
        if (p1 > (seq_t)(n / 3)) break;

        seq_t max_p2 = ((seq_t)n - p1) / 2;

        for (idx_t j = i; j < pcount; ++j)
        {
            seq_t p2 = pal[j];
            if (p2 > max_p2) break;

            seq_t p3 = ((seq_t)n - p1 - p2);
            if (p3 < p2) continue;

            if (is_pal(p3))
                ++result;
        }
    }

    return result;
}
