/*
 * A263610  Palindromes in base 4 which are also squares.
 * https://oeis.org/A263610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromes in base 4 which are also squares.
   a(n) = the n‑th integer whose base‑4 representation is a palindrome
   and whose decimal value is a perfect square.  The function returns that
   integer written in decimal (i.e. the base‑4 digits interpreted as a
   base‑10 number). */
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many terms found so far      */
    idx_t m     = 0;          /* current integer being squared    */

    /* static array large enough for all expected digit lengths   */
    static seq_t digits[2048];

    while (count < n)
    {
        seq_t m_seq = (seq_t)m;
        seq_t sq    = seq_mul(m_seq, m_seq);   /* m^2 without overflow   */

        /* Convert sq to base‑4 digits   */
        seq_t t = sq;
        idx_t pos = 0;
        if (t == 0)
        {
            digits[pos++] = 0;          /* special case: 0 */
        }
        else
        {
            while (t > 0)
            {
                digits[pos++] = t % 4;  /* lowest digit first   */
                t /= 4;
            }
        }

        /* Test whether the base‑4 digits form a palindrome */
        bool pal = true;
        for (idx_t i = 0; i < pos / 2; ++i)
        {
            if (digits[i] != digits[pos - 1 - i]) { pal = false; break; }
        }

        if (pal)
        {
            ++count;
            if (count == n)
            {
                /* Convert the base‑4 digits to the decimal integer
                   that the sequence records (the digits interpreted
                   as decimal digits). */
                seq_t val = 0;
                for (idx_t i = pos; i > 0; --i)
                {
                    val = val * 10 + digits[i - 1];
                }
                return val;
            }
        }
        ++m;      /* next integer to square   */
    }

    return 0;    /* never reached          */
}
