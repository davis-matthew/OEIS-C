/*
 * A357634  Skew-alternating sum of the partition having Heinz number n.
 * https://oeis.org/A357634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = skew‑alternating sum of the partition * whose Heinz
 *            number is n. * * The partition is obtained by factorising n = ∏
 *            p_k^e_k, * mapping each prime p_k to its index in the sequence
 *            of primes * (2→1, 3→2, 5→3, …), and taking e_k copies of that
 *            index. * The parts are sorted in non‑increasing o...
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* array of primes up to prime_limit */
static size_t     prime_cnt = 0;    /* number of primes stored */
static uint64_t   prime_limit = 0;  /* largest prime currently stored */

static void _ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;
    if (primes)
        free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* linear search for prime index (primes array is sorted) */
static size_t _prime_index(uint64_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i)
        if (primes[i] == p)
            return i + 1;          /* indices are 1‑based in OEIS */
    /* Should never happen if primes list contains p. */
    return 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;           /* a(1) = 0 */

    uint64_t un = (uint64_t)n;
    _ensure_primes(un);            /* need primes up to n for indexing */

    uint64_t m = un;
    /* sufficient for n up to 10⁶; array large enough for 10000 */
    uint32_t parts[128];
    int parts_len = 0;

    /* factor using the prime list */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i)
    {
        uint64_t p = primes[i];
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            for (int k = 0; k < exp; ++k)
                parts[parts_len++] = (uint32_t)(i + 1);   /* prime index */
        }
    }

    if (m > 1)   /* remaining prime factor */
    {
        _ensure_primes(m);                     /* ensure its index is known */
        size_t idx = _prime_index(m);
        for (int k = 0; k < 1; ++k)
            parts[parts_len++] = (uint32_t)(idx);
    }

    /* sort in non‑increasing order (insertion sort, small size) */
    for (int i = 1; i < parts_len; ++i)
    {
        uint32_t key = parts[i];
        int j = i - 1;
        while (j >= 0 && parts[j] < key)
        {
            parts[j + 1] = parts[j];
            --j;
        }
        parts[j + 1] = key;
    }

    /* compute skew‑alternating sum with period 4 */
    seq_t sum = 0;
    for (int i = 0; i < parts_len; ++i)
    {
        int rem = i & 3;                     /* i % 4 */
        seq_t val = (seq_t)parts[i];
        if (rem == 1 || rem == 2)
            sum -= val;
        else
            sum += val;
    }
    return sum;
}
