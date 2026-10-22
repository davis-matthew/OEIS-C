/*
 * A316603  Double prime times.
 * https://oeis.org/A316603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A316603: Numbers on a 4‑digit hh:mm digital clock that remain prime
 * when the display mode is switched between 12‑hour AM/PM and 24‑hour
 * time. The function enumerates all valid times, keeps those numbers
 * that appear in *every* time it is displayed with the counterpart
 * also prime, and returns the n‑th such number (1‑based indexing). */

#include <stdbool.h>
#include <stddef.h>

/* Smallest‑prime‑factor helper (fail if n < 2). */
static bool is_prime_small(unsigned long long n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (unsigned long long d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* Sequence value accessor. */
seq_t a(idx_t n)
{
    /* static cache for the sequence values. */
    static seq_t seq[2400];
    static size_t seq_len = 0;
    static bool built = false;

    if (!built)
    {
        bool present[2400] = {false};
        bool invalid[2400] = {false};

        for (idx_t h = 0; h < 24; ++h)
        {
            for (idx_t m = 0; m < 60; ++m)
            {
                idx_t val24 = h * 100 + m;
                idx_t hour12 = h % 12;          /* mod 12 */
                if (hour12 == 0) hour12 = 12;   /* 12‑hour mapping */
                idx_t val12 = hour12 * 100 + m;

                bool p24 = is_prime_small((unsigned long long)val24);
                bool p12 = is_prime_small((unsigned long long)val12);

                if (p24 && p12)
                {
                    if (val24 == val12)
                        present[val24] = true;
                    else
                    {
                        present[val24] = true;
                        present[val12] = true;
                    }
                }
                else if (p24 != p12) /* only one prime – mark the prime one invalid */
                {
                    if (p24 && !p12) invalid[val24] = true;
                    if (p12 && !p24) invalid[val12] = true;
                }
                /* both composite → nothing to do */
            }
        }

        /* Now build the sorted list, excluding invalid numbers. */
        for (idx_t i = 0; i < 2400; ++i)
        {
            if (present[i] && !invalid[i])
                seq[seq_len++] = (seq_t)i;
        }

        built = true;
    }

    if (n == 0 || (size_t)n > seq_len) return 0;
    return seq[(size_t)n - 1];
}
