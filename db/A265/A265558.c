/*
 * A265558  a(n) = smallest base-10 palindrome m >= n such that every base-10
 *          digit of n is <= the corresponding digit of m.
 * https://oeis.org/A265558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-10 palindrome m ≥ n such that every decimal
 *            digit of n is ≤ the corresponding digit of m
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Convert a number to its base‑10 digits stored least‑significant first. */
static void get_digits_seq(seq_t x, uint8_t *buf, size_t *len)
{
    size_t l = 0;
    if (x == 0)
    {
        buf[l++] = 0;
    }
    else
    {
        while (x != 0)
        {
            buf[l++] = (uint8_t)(x % 10);
            x /= 10;
        }
    }
    *len = l;
}

/* Return the required sequence element. */
seq_t a(idx_t n)
{
    /* digits of n, least‑significant first */
    uint8_t nd[64];
    size_t len_n = 0;
    get_digits_seq((seq_t)n, nd, &len_n);

    seq_t m = (seq_t)n;            /* start search at n itself */
    for (;; m++)
    {
        uint8_t md[64];
        size_t len_m = 0;
        get_digits_seq(m, md, &len_m);

        /* check palindrome: md[i] must equal md[len_m-1-i] */
        bool is_pal = true;
        for (size_t i = 0; i + i < len_m; ++i)
        {
            if (md[i] != md[len_m - 1 - i])
            {
                is_pal = false;
                break;
            }
        }
        if (!is_pal) continue;

        /* digit‑wise comparison of n and m (same offset from the right) */
        bool digit_ok = true;
        for (size_t i = 0; i < len_n; ++i)
        {
            if (nd[i] > md[i])
            {
                digit_ok = false;
                break;
            }
        }
        if (digit_ok) return m;
    }
}
