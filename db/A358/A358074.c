/*
 * A358074  a(n) is the number of distinct ways n can be written as the sum of
 *          a perfect square and factorial.
 * https://oeis.org/A358074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct ways n can be written as the sum
 *            of a perfect square and a factorial, treating 0! and 1! as the
 *            same value and counting each square with its non‑negative root
 *            only
 * Range:     u8 n <= 64, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* factorials <= 20! (since 21! exceeds 2^64-1).  0! and 1! are
       combined into a single value because they are equal. */
    static const idx_t fact_vals[] = {
        1ULL,          /* 0! = 1! = 1 */
        2ULL,          /* 2! */
        6ULL,          /* 3! */
        24ULL,         /* 4! */
        120ULL,        /* 5! */
        720ULL,        /* 6! */
        5040ULL,       /* 7! */
        40320ULL,      /* 8! */
        362880ULL,     /* 9! */
        3628800ULL,    /* 10! */
        39916800ULL,   /* 11! */
        479001600ULL,  /* 12! */
        6227020800ULL, /* 13! */
        87178291200ULL,/* 14! */
        1307674368000ULL,/* 15! */
        20922789888000ULL,/* 16! */
        355687428096000ULL,/* 17! */
        6402373705728000ULL,/* 18! */
        121645100408832000ULL,/* 19! */
        2432902008176640000ULL/* 20! */
    };
    const idx_t nfacts = sizeof(fact_vals) / sizeof(fact_vals[0]);

    seq_t count = 0;
    for (idx_t i = 0; i < nfacts; ++i) {
        if (fact_vals[i] > n)
            break;                      /* factorial too large for this n */
        seq_t remain = (seq_t)n - (seq_t)fact_vals[i];
        seq_t sq = floor_sqrt(remain);
        if (sq * sq == remain)
            ++count;
    }

    return count;
}
