/*
 * A378873  Squarefree part of A378872(n) (the discriminant of the minimal
 *          polynomial of a number whose continued fraction expansion has
 *          periodic part given by the n-th composition (in standard order)).
 * https://oeis.org/A378873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree part of the discriminant of the quadratic
 *            irrational whose pure periodic continued fraction has the n‑th
 *            composition (standard order). If the composition is a1…ak then M
 *            = Π_{i=1..k} [[ai,1],[1,0]] with M = [[A,B],[C,D]], the
 *            discriminant is Δ = (D-A)^2 + 4·B·C. Return the squar...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* remove all square factors from x (x is a 64‑bit unsigned integer). */
static unsigned long long squarefree_part(unsigned long long x)
{
    for (unsigned long long p = 2; p * p <= x; ++p)
    {
        unsigned long long sq = p * p;
        while (x % sq == 0)
            x /= sq;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1 */

    /* Find the sum k of the composition: locate the smallest k with
       2^k - 1 >= n  ->  total compositions up to sum k. */
    idx_t k = 1;
    idx_t pow2 = 2;                 /* 2^k */
    while (pow2 - 1 < n)
    {
        ++k;
        pow2 <<= 1;
    }

    /* Number of compositions with a sum strictly less than k. */
    idx_t total_before = (pow2 >> 1) - 1;   /* 2^(k-1)-1 */
    /* 0–based index of the composition among all sums equal to k. */
    idx_t idx = n - total_before - 1;       /* 0 .. 2^(k-1)-1 */

    /* Decode the composition from the binary representation. */
    seq_t parts[64];       /* k ≤ 64 for all n up to the required limit */
    size_t len = 0;
    seq_t current = 1;

    for (int pos = (int)k - 2; pos >= 0; --pos)
    {
        bool bit = (idx >> pos) & 1ULL;
        if (bit)
        {
            parts[len++] = current;
            current = 1;
        }
        else
            current += 1;
    }
    parts[len++] = current;       /* final part */

    /* Compute the matrix M = Π [[ai,1],[1,0]] from left to right. */
    seq_t A = 1, B = 0, C = 0, D = 1;   /* identity matrix */

    for (size_t i = 0; i < len; ++i)
    {
        seq_t ai = parts[i];
        seq_t nA = A * ai + B;
        seq_t nB = A;
        seq_t nC = C * ai + D;
        seq_t nD = C;
        A = nA; B = nB; C = nC; D = nD;
    }

    /* discriminant Δ = (D-A)^2 + 4*B*C  (use 128‑bit to avoid overflow). */
    unsigned long long diff = (D > A) ? (unsigned long long)(D - A)
                                     : (unsigned long long)(A - D);
    unsigned __int128 part1 = (unsigned __int128)diff * (unsigned __int128)diff;
    unsigned __int128 part2 = (unsigned __int128)4 *
                            (unsigned __int128)(unsigned long long)B *
                            (unsigned __int128)(unsigned long long)C;
    unsigned __int128 disc128 = part1 + part2;

    unsigned long long disc = (unsigned long long)disc128; /* fits for n ≤ 10000 */

    /* remove square divisors to get the squarefree part */
    unsigned long long result = squarefree_part(disc);

    return (seq_t)result;
}
