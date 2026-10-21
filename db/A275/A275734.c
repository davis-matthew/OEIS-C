/*
 * A275734  Prime-factorization representations of "factorial base slope
 *          polynomials": a(0) = 1; for n >= 1, a(n) = A275732(n) *
 *          a(A257684(n)).
 * https://oeis.org/A275734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; For n ≥ 1, write n in factorial base n = ∑_{k=1}^{∞}
 *            d_k · k! (with 0 ≤ d_k ≤ k). For each position k where d_k > 0
 *            let level = k – d_k (0 ≤ level ≤ k‑1). Then a(n) = ∏_{k=1}^{∞}
 *            prime(k+1) ^ (number of digits of level k‑1). In other words
 *            a(n) = ∏_{k=1}^{∞} prime(k+1) ^ { #{ i : i>0, l...
 * Range:     u8 n <= 128, u16 n <= 5888, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

static const idx_t FACT[21] = {
    1ULL,                     /* 0! */
    1ULL,                     /* 1! */
    2ULL,                     /* 2! */
    6ULL,                     /* 3! */
    24ULL,                    /* 4! */
    120ULL,                   /* 5! */
    720ULL,                   /* 6! */
    5040ULL,                  /* 7! */
    40320ULL,                 /* 8! */
    362880ULL,                /* 9! */
    3628800ULL,               /*10! */
    39916800ULL,              /*11! */
    479001600ULL,             /*12! */
    6227020800ULL,            /*13! */
    87178291200ULL,           /*14! */
    1307674368000ULL,         /*15! */
    20922789888000ULL,        /*16! */
    355687428096000ULL,       /*17! */
    6402373705728000ULL,      /*18! */
    121645100408832000ULL    /*19! */
    /* 20! would overflow uint64_t; no need for our n ≤ 10000 */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Find largest m with m! ≤ n   (m ≤ 9 for n ≤ 10000) */
    int m = 1;
    while (m < 20 && FACT[m] <= n) m++;
    m--;  /* now FACT[m] ≤ n < FACT[m+1] */

    /* Count levels */
    /* level ranges from 0 to m-1, allocate m slots + 1 for safety */
    uint64_t level_count[21] = {0};

    idx_t remaining = n;
    for (int pos = m; pos >= 1; pos--)
    {
        idx_t f = FACT[pos];
        idx_t digit = remaining / f;
        remaining -= digit * f;

        if (digit > 0)
        {
            /* level = pos - digit (0‑based) */
            uint64_t level = (uint64_t)pos - (uint64_t)digit;
            level_count[level]++;
        }
    }

    /* Build result */
    seq_t result = (seq_t)1;

    for (int lvl = 0; lvl <= m; lvl++)
    {
        uint64_t cnt = level_count[lvl];
        if (cnt == 0) continue;

        seq_t prime = get_prime((idx_t)(lvl + 1));      /* prime(k+1) */
        seq_t pw = big_pow(prime, (seq_t)cnt);          /* prime^(cnt) */
        result *= pw;
    }

    return result;
}
