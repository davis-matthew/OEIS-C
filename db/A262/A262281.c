/*
 * A262281  a(n) = smallest nonnegative number, not a power of 2, that is not
 *          a substring of n in its binary representation.
 * https://oeis.org/A262281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest nonnegative, non‑power‑of‑two integer not
 *            occurring as a binary substring of n (binary representation of
 *            n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static inline bool is_power_of_two_seq(seq_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* Convert n to a binary string bits_n of length L (most significant bit first). */
    size_t L;
    idx_t tmp = n;
    if (tmp == 0)
        L = 1;
    else
    {
        L = 0;
        while (tmp != 0)
        {
            L++;
            tmp >>= 1;
        }
    }

    char bits_n[65];  /* L <= 64 for 64‑bit idx_t, +1 for n==0 */
    tmp = n;
    if (tmp == 0)
    {
        bits_n[0] = '0';
    }
    else
    {
        for (size_t i = L; i-- > 0; )
        {
            bits_n[i] = (tmp & 1) ? '1' : '0';
            tmp >>= 1;
        }
    }

    /* Iterate over candidate m */
    seq_t m = 0;
    for (;; ++m)
    {
        if (is_power_of_two_seq(m))
            continue;   /* ignore powers of two */

        /* Binary representation of m. */
        size_t len_m;
        seq_t tmpm = m;
        if (tmpm == 0)
        {
            len_m = 1;
        }
        else
        {
            len_m = 0;
            while (tmpm != 0)
            {
                len_m++;
                tmpm >>= 1;
            }
        }

        /* If m is longer than n, it cannot be a substring. */
        if (len_m > L)
            return m;

        /* Build bits_m string. */
        char bits_m[65];
        tmpm = m;
        for (size_t i = len_m; i-- > 0; )
        {
            bits_m[i] = (tmpm & 1) ? '1' : '0';
            tmpm >>= 1;
        }

        /* Search for bits_m as a contiguous substring of bits_n. */
        bool found = false;
        for (size_t pos = 0; pos + len_m <= L; ++pos)
        {
            bool ok = true;
            for (size_t k = 0; k < len_m; ++k)
            {
                if (bits_n[pos + k] != bits_m[k])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                found = true;
                break;
            }
        }

        if (!found)
            return m;  /* m is the smallest required number */
    }
}
