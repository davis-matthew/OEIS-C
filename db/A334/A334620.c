/*
 * A334620  a(n) is the smallest multiple of n formed by the concatenation
 *          1,2,3,...,k for some k.
 * https://oeis.org/A334620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the smallest multiple of n that appears as the *
 *            concatenation 1,2,3,…,k for some k. This implementation * finds
 *            the smallest k by walking the sequence modulo n and then *
 *            constructs the full concatenated decimal string to convert to
 *            seq_t
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

/* number of decimal digits of an idx_t */
static int digits_of(idx_t x)
{
    int d = 0;
    do
    {
        ++d;
        x /= 10;
    } while (x > 0);
    return d;
}

seq_t a(idx_t n)
{
    /* n = 0 is out of range for this sequence, but handle it safely */
    if (n == 0)
        return (seq_t)0;

    /* find the smallest k such that the concatenation 1..k is a multiple of n */
    idx_t mod = 0;
    idx_t k = 0;
    for (;;)
    {
        ++k;
        /* compute (current_value * 10^digits(k) + k) mod n */
        int d = digits_of(k);
        idx_t pow10 = 1 % n;
        for (int i = 0; i < d; ++i)
            pow10 = (pow10 * 10) % n;
        mod = (mod * pow10 + k % n) % n;
        if (mod == 0)
            break;
    }

    /* now build the concatenated string 1..k */
    size_t cap = 64;
    size_t len = 0;
    char *buf = (char *)malloc(cap);
    if (!buf)
        return (seq_t)0;        /* allocation failure unlikely */

    for (idx_t i = 1; i <= k; ++i)
    {
        /* convert i to decimal representation */
        idx_t tmp = i;
        char small[20];
        int pos = 0;
        if (tmp == 0)
        {
            small[pos++] = '0';
        }
        else
        {
            while (tmp > 0)
            {
                small[pos++] = '0' + (tmp % 10);
                tmp /= 10;
            }
            /* reverse the digits to correct order */
            for (int a = 0; a < pos / 2; ++a)
            {
                char c = small[a];
                small[a] = small[pos - 1 - a];
                small[pos - 1 - a] = c;
            }
        }

        /* ensure buffer has enough space */
        if (len + pos > cap)
        {
            cap = cap * 2 + pos;
            char *new_buf = (char *)realloc(buf, cap);
            if (!new_buf)
            {
                free(buf);
                return (seq_t)0;   /* allocation failure */
            }
            buf = new_buf;
        }
        memcpy(buf + len, small, pos);
        len += pos;
    }

    /* finish string, convert to seq_t, and clean up */
    char *final_buf = (char *)malloc(len + 1);
    if (!final_buf)
    {
        free(buf);
        return (seq_t)0;
    }
    memcpy(final_buf, buf, len);
    final_buf[len] = '\0';
    free(buf);

    seq_t result = seq_from_str(final_buf);
    free(final_buf);
    return result;
}
