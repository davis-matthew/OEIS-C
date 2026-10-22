/*
 * A359074  Numbers that have at least two divisors with an equal sum of
 *          digits.
 * https://oeis.org/A359074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that has at least two distinct
 *            divisors with the same sum of decimal digits
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Digit sum for a 64‑bit unsigned integer. */
static inline unsigned int digit_sum_u64(uint64_t v)
{
    unsigned int sum = 0;
    while (v > 0)
    {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Test whether a number has two divisors with equal digit sum. */
static bool has_property(uint64_t n)
{
    /* The maximum possible digit sum for a 64‑bit number is 19 * 9 = 171. */
    enum { MAX_SUM = 200 };
    bool seen[MAX_SUM] = {0};

    for (uint64_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            unsigned int s1 = digit_sum_u64(d);
            if (seen[s1]) return true;
            seen[s1] = true;

            uint64_t d2 = n / d;
            if (d2 != d)
            {
                unsigned int s2 = digit_sum_u64(d2);
                if (seen[s2]) return true;
                seen[s2] = true;
            }
        }
    }
    return false;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static uint64_t next_num = 1;   /* number to be tested next */

    /* Grow the term array as needed. */
    if (terms_cap == 0)
    {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_size < n)
    {
        /* Find the next qualifying number. */
        do
        {
            ++next_num;
        } while (!has_property(next_num));

        /* Store the found term. */
        if (terms_size == terms_cap)
        {
            terms_cap *= 2;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_size++] = (seq_t)next_num;
    }

    return terms[n - 1];
}
