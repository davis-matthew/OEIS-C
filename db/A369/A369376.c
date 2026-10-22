/*
 * A369376  a(n) is the number of elements p(j) > j (right displacements) in
 *          the n-th permutation in lexicographic order.
 * https://oeis.org/A369376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9285, u16 n <= 9285, u32 n <= 9285, u64 n <= 9285,
 *            u128 n <= 9285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of right displacements in the n‑th permutation in lexicographic order.
   The permutation is obtained by the factoradic representation of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* factorials up to 20! (sufficient for all n ≤ 10000 and beyond for 64‑bit limits) */
    static const idx_t fact[21] = {
        1ULL,               /* 0! */
        1ULL,               /* 1! */
        2ULL,               /* 2! */
        6ULL,               /* 3! */
        24ULL,              /* 4! */
        120ULL,             /* 5! */
        720ULL,             /* 6! */
        5040ULL,            /* 7! */
        40320ULL,           /* 8! */
        362880ULL,          /* 9! */
        3628800ULL,         /* 10! */
        39916800ULL,        /* 11! */
        479001600ULL,       /* 12! */
        6227020800ULL,      /* 13! */
        87178291200ULL,     /* 14! */
        1307674368000ULL,   /* 15! */
        20922789888000ULL,  /* 16! */
        355687428096000ULL, /* 17! */
        6402373705728000ULL,/* 18! */
        121645100408832000ULL, /* 19! */
        2432902008176640000ULL  /* 20! */
    };

    /* Determine the length k of the permutation: smallest k with k! > n */
    idx_t k = 1;
    while (fact[k] <= n)
        ++k;        /* for n ≤ 10000, k never exceeds 9; for 64‑bit n it never exceeds 21 */

    /* Factoradic digits of n: digit[i] corresponds to factorial i!      */
    /* The array size never exceeds 21, so a fixed static array is safe. */
    idx_t digit[21] = {0};
    idx_t r = n;
    for (idx_t i = k; i > 0; --i) {
        idx_t pos = i - 1;           /* factorial index */
        digit[pos] = r / fact[pos];
        r           = r % fact[pos];
    }

    /* Build the permutation and count right displacements. */
    idx_t elems[21];
    for (idx_t i = 0; i < k; ++i)
        elems[i] = i;

    seq_t count = 0;
    idx_t len = k;
    for (idx_t pos = 0; pos < k; ++pos) {
        /* Use the most significant digit first: digit[k‑1‑pos] */
        idx_t idx = digit[k - 1 - pos];
        idx_t val = elems[idx];
        if (val > pos)
            ++count;

        /* Remove the used element from the list */
        for (idx_t t = idx; t + 1 < len; ++t)
            elems[t] = elems[t + 1];
        --len;
    }

    return count;
}
