/*
 * A378080  Nonnegative integers that are not digitally balanced in any
 *          integer base b >= 2.
 * https://oeis.org/A378080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A378080: Nonnegative integers that are not digitally balanced
   in any base b ≥ 2.  The function performs a brute‑force search for
   each candidate integer, testing all bases up to its digit length. */
/* This implementation works for any seq_t/idx_t types supplied by the
   build environment (C11/C++17). */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff x is digitally balanced in some base 2 ≤ b ≤ 128. */
static bool is_balanced(idx_t x)
{
    if (x == 0)                /* 0 is never balanced in any base */
        return false;

    /* The maximum digit length for a 64‑bit integer is < 65, so a base
       larger than 128 can never be balanced. */
    for (idx_t base = 2; base <= 128; ++base)
    {
        idx_t t  = x;
        int   cnt[128] = {0};
        int   len = 0;

        /* Extract digits of x in this base. */
        if (t == 0)
        {
            cnt[0] = 1;
            len = 1;
        }
        else
        {
            while (t > 0)
            {
                cnt[t % base]++;
                t /= base;
                ++len;
            }
        }

        /* A digitally balanced number must have len divisible by base. */
        if (len % base != 0)
            continue;

        int k = len / base;          /* expected count of each digit */

        bool ok = true;
        for (int d = 0; d < base; ++d)
        {
            if (cnt[d] != k)
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;             /* balanced in this base */
    }
    return false;                    /* not balanced in any examined base */
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)            /* out of range, but return 0 as a safe fallback */
        return 0;

    idx_t candidate = 0;
    idx_t found = 0;

    while (found < n)
    {
        if (!is_balanced(candidate))
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Unreachable, but keep compiler quiet. */
    return 0;
}
