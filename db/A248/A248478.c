/*
 * A248478  Evil numbers (A001969) becoming odious (A000069) if any digit is
 *          deleted (zeros allowed).
 * https://oeis.org/A248478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number that is evil (even popcount) and every
 *            single-digit deletion yields an odious number (odd popcount)
 * Range:     u8 n <= 27, u16 n <= 928, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: popcount of a seq_t value */
static unsigned int popcount_seq(seq_t v)
{
    unsigned int c = 0;
    while (v)
    {
        v &= v - 1;
        ++c;
    }
    return c;
}

/* helper: number of decimal digits of a seq_t value */
static int decimal_len(seq_t v)
{
    int len = 0;
    while (v)
    {
        v /= 10;
        ++len;
    }
    return len;
}

/* helper: access powers of 10 (static table) */
static seq_t power10(int k)
{
    static seq_t p[25];
    static int init = 0;
    if (!init)
    {
        p[0] = 1;
        for (int i = 1; i < 25; ++i)
            p[i] = p[i - 1] * 10;
        init = 1;
    }
    return p[k];
}

/* test whether a number satisfies the sequence property */
static bool good(seq_t x)
{
    if (popcount_seq(x) % 2 != 0)  /* must be evil */
        return false;

    int len = decimal_len(x);
    if (len <= 1)  /* single-digit numbers always fail (no digit to delete) */
        return false;

    for (int i = 0; i < len; ++i)          /* delete digit i from left to right */
    {
        seq_t mult = power10(len - i - 1);
        seq_t div  = power10(len - i);
        seq_t left  = x / div;
        seq_t right = x % mult;
        seq_t y = left * mult + right;

        if (popcount_seq(y) % 2 == 0)   /* must become odious */
            return false;
    }
    return true;
}

/* sequence accessor */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t seen  = 0;
    static size_t cap   = 0;
    static uint64_t cand = 1;          /* candidate numbers, 64‑bit  */

    /* generate terms until we have at least n */
    while (seen < n)
    {
        seq_t v = (seq_t)cand;
        if (good(v))
        {
            if (seen == cap)
            {
                size_t newcap = (cap ? cap * 2 : 16);
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp)
                    exit(1);          /* out of memory */
                terms = tmp;
                cap   = newcap;
            }
            terms[seen++] = v;
        }
        ++cand;
    }
    return terms[n - 1];
}
