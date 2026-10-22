/*
 * A392375  Numbers k such that k is coprime to each of its digits and to the
 *          sum of its digits.
 * https://oeis.org/A392375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392375: numbers k such that k is coprime to each of its digits and to the sum of its digits. */
#include "../utils/gcd.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many terms have been found so far */
    seq_t k = 1;              /* candidate number */

    while (1)
    {
        bool ok = true;
        seq_t temp = k;
        seq_t sum = 0;

        /* Examine each decimal digit of k */
        while (temp > 0)
        {
            seq_t d = temp % (seq_t)10;
            if (d == 0) {             /* digit zero ⇒ gcd(k,0)=k>1 unless k==1 */
                ok = false;
                break;
            }
            if (gcd(k, d) != (seq_t)1)
            {
                ok = false;
                break;
            }
            sum += d;
            temp /= (seq_t)10;
        }

        if (ok && gcd(k, sum) != (seq_t)1)
            ok = false;

        if (ok)
        {
            count++;
            if (count == n)
                return k;
        }

        ++k;
    }
}
