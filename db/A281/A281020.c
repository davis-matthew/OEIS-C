/*
 * A281020  Numbers with a prime number of raised dots in their Braille
 *          representation.
 * https://oeis.org/A281020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with a prime number of raised dots in their Braille representation.
   The total dot count for a number x is 4 (the number sign) plus the dot
   counts of each decimal digit according to the standard a–j mapping. */

#include <stdbool.h>

/* Braille dot counts for decimal digits 0–9 using the standard a–j mapping.
   0 (⠚): 3 dots  1 (⠁): 1 dot  2 (⠃): 2 dots  3 (⠉): 2 dots
   4 (⠙): 3 dots  5 (⠑): 2 dots  6 (⠋): 3 dots  7 (⠛): 4 dots
   8 (⠓): 3 dots  9 (⠊): 2 dots */
static const unsigned char dot_counts[10] =
{
    3,  /* 0 */
    1,  /* 1 */
    2,  /* 2 */
    2,  /* 3 */
    3,  /* 4 */
    2,  /* 5 */
    3,  /* 6 */
    4,  /* 7 */
    3,  /* 8 */
    2   /* 9 */
};

/* Test whether a small integer d (expected < 200) is prime. */
static bool is_prime_small(int d)
{
    if (d < 2) return false;
    if (d % 2 == 0) return d == 2;
    for (int i = 3; i * i <= d; i += 2)
    {
        if (d % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;  /* guard for accidental n==0 */

    idx_t found = 0;          /* how many qualifying numbers seen */
    seq_t x = 0;              /* current candidate */

    while (true)
    {
        /* compute total dot count: fixed 4 for the number sign plus digit */
        int dot_total = 4;
        seq_t tmp = x;
        if (tmp == 0)
        {
            dot_total += dot_counts[0];
        }
        else
        {
            while (tmp > 0)
            {
                int digit = (int)(tmp % 10);
                dot_total += dot_counts[digit];
                tmp /= 10;
            }
        }

        if (is_prime_small(dot_total))
        {
            ++found;
            if (found == n) return x;
        }

        ++x;  /* next integer */
    }
}
