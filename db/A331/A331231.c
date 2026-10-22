/*
 * A331231  Numbers k such that the number of factorizations of k into
 *          distinct factors > 1 is even.
 * https://oeis.org/A331231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating numbers k and checking the parity of the
   number of distinct-factor factorizations (factors > 1).  A number k is
   included in the sequence iff that count is even. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* dynamic array storing the sequence values */
static seq_t *seq = NULL;
static idx_t  seq_len = 0;
static idx_t  seq_cap = 0;

/* advance candidate integer */
static uint64_t next_k = 2;

/* append a value to the dynamic array */
static void seq_append(seq_t val)
{
    if (seq_len >= seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory – let the test fail */
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* integer square root of a 64‑bit unsigned integer */
static uint64_t isqrt(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) low = mid;
        else         high = mid - 1;
    }
    return low;
}

/* parity of the number of distinct-factor factorizations of n
   (factoring with all factors > 1 and distinct, order disregarded).
   The recursion starts with the smallest permissible factor. */
static int parity_rec(uint64_t n, uint64_t min_factor)
{
    if (n == 1)            /* finished a factorization */
        return 1;          /* counts as one factorization (the empty choice) */

    int parity = 0;
    uint64_t dmax = isqrt(n);

    for (uint64_t d = min_factor; d <= dmax; ++d) {
        if (n % d == 0)
            parity ^= parity_rec(n / d, d + 1);
    }

    /* include the factorization consisting solely of n itself */
    if (n >= min_factor)
        parity ^= 1;

    return parity;           /* 0 = even number of factorizations, 1 = odd */
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* behavior for indices < 1 is irrelevant */

    /* generate terms until we have enough to answer n */
    while (seq_len < n) {
        if ((parity_rec(next_k, 2) & 1) == 0)   /* even parity → include */
            seq_append((seq_t)next_k);
        ++next_k;
    }
    return seq[n - 1];
}
