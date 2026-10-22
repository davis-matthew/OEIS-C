/*
 * A392120  Numbers k such that (k^3 - greatest prime < k^3) < (-k^3 + least
 *          prime > k^3).
 * https://oeis.org/A392120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 23, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A392120
    a(n) = the nth integer k ≥ 1 such that
           (k³ – greatest prime < k³) < (‑k³ + least prime > k³)
    Equivalently, let P₋(x) be the largest prime < x and
                      P₊(x) the smallest prime > x;
    then a(n) is the nth k satisfying  P₋(k³) + P₊(k³)  >  2·k³ .
    Offset : 1
*/

#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

static seq_t prev_prime(seq_t x)
/* return the largest prime < x; 0 if none */
{
    if (x <= (seq_t)2)
        return (seq_t)0;
    seq_t p = x - (seq_t)1;
    while (!is_prime_big(p))
        p -= (seq_t)1;
    return p;
}

static seq_t next_prime(seq_t x)
/* return the smallest prime  > x */
{
    seq_t p = x + (seq_t)1;
    while (!is_prime_big(p))
        p += (seq_t)1;
    return p;
}

seq_t a(idx_t n)
/* Sequence A392120: nth term (offset 1) */
{
    if (n == 0)   /* offset 1 – undefined for n = 0 */
        return (seq_t)0;

    idx_t found = 0;   /* how many terms found so far     */
    seq_t k = (seq_t)1;   /* candidate k, start at 1         */

    while (found < n)
    {
        /* cube of k */
        seq_t cube = k * k * k;

        /* nearest primes around k³ */
        seq_t low  = prev_prime(cube);   /* largest prime < k³ */
        seq_t high = next_prime(cube);   /* smallest prime > k³ */

        /* test the condition P₋ + P₊  >  2·k³ */
        if (low + high > (seq_t)2 * cube)
        {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }

    /* unreachable – just to keep the compiler happy */
    return (seq_t)0;
}
