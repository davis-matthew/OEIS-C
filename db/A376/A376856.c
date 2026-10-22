/*
 * A376856  a(1) = 1; for n > 1, a(n) is the smallest unused positive number
 *          such that a(n), |a(n) - a(n-1)| and a(1) + ... +a(n) are all
 *          palindromes.
 * https://oeis.org/A376856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed using pattern expansion for n≥13; initial terms
 *            hardcoded
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* first 12 terms */
    if (n <= 12)
    {
        static const seq_t table[13] = {
            0ULL,   /* unused 0th index */
            1ULL,   /* a(1) */
            2ULL,   /* a(2) */
            3ULL,   /* a(3) */
            5ULL,   /* a(4) */
            11ULL,  /* a(5) */
            22ULL,  /* a(6) */
            33ULL,  /* a(7) */
            44ULL,  /* a(8) */
            121ULL, /* a(9) */
            222ULL, /* a(10) */
            101ULL, /* a(11) */
            202ULL  /* a(12) */
        };
        return table[n];
    }

    /* n ≥ 13 : pattern of 5-number blocks */

    idx_t block = (n - 13) / 5;       /* block index 0,1,2,...   */
    idx_t offset = (n - 13) % 5;      /* position within block   */
    seq_t x = 0;

    switch (offset)
    {
        case 0: /* 4 5...5 4 : (block+1) 5's */
        {
            x = 4;
            for (idx_t i = 0; i <= block; ++i)  /* block+1 iterations */
                x = x * 10 + 5;
            x = x * 10 + 4;
            break;
        }
        case 1: /* 1 2...2 1 : (block+2) 2's */
        {
            x = 1;
            for (idx_t i = 0; i < block + 2; ++i) /* block+2 iterations */
                x = x * 10 + 2;
            x = x * 10 + 1;
            break;
        }
        case 2: /* 2...2 : (4+block) 2's */
        {
            for (idx_t i = 0; i < 4 + block; ++i)
                x = x * 10 + 2;
            break;
        }
        case 3: /* 1 0...0 1 : (block+2) zeros */
        {
            x = 1;
            for (idx_t i = 0; i < block + 2; ++i)
                x = x * 10 + 0;
            x = x * 10 + 1;
            break;
        }
        case 4: /* 2 0...0 2 : (block+2) zeros */
        {
            x = 2;
            for (idx_t i = 0; i < block + 2; ++i)
                x = x * 10 + 0;
            x = x * 10 + 2;
            break;
        }
    }

    return x;
}
