/*
 * A373482  Numbers k for which A003415(k) is a multiple of A001414(k), where
 *          A003415 is the arithmetic derivative, and A001414 is the sum of
 *          prime factors with multiplicity.
 * https://oeis.org/A373482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 4660, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373482: numbers k such that A003415(k) is a multiple of A001414(k). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Smallest prime factor table – grows dynamically. */
static uint64_t *spf = NULL;
static size_t spf_size = 0;

/* Resize SPF table so that indices 0…limit are valid. */
static void build_spf_to(size_t limit)
{
    if (limit + 1 <= spf_size) return;   /* already big enough */
    free(spf);
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!spf) abort();
    for (size_t i = 0; i <= limit; ++i) spf[i] = i;
    spf[0] = 0; spf[1] = 1;
    for (size_t i = 2; (uint64_t)i * i <= limit; ++i)
    {
        if (spf[i] == i)               /* i is prime */
        {
            uint64_t ii = (uint64_t)i * i;
            for (uint64_t j = ii; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_size = limit + 1;
}

/* Check whether integer k satisfies A003415(k) % A001414(k) == 0. */
static bool qualifies(idx_t k)
{
    if (k == 1) return true;            /* 1 is always a member */

    seq_t sum_pf = 0;   /* sum of prime factors with multiplicity */
    seq_t d      = 0;   /* arithmetic derivative */

    uint64_t n = (uint64_t)k;
    while (n > 1)
    {
        uint64_t p = spf[n];
        size_t e = 0;
        while (n % p == 0) { n /= p; ++e; }

        sum_pf += (seq_t)e * (seq_t)p;
        d += (seq_t)e * (seq_t)k / (seq_t)p;
    }
    return (d % sum_pf) == 0;
}

/* Sequence storage – grows as needed. */
static seq_t *seq_arr   = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static idx_t  next_k    = 1;          /* next integer to test */

/* Ensure seq_arr can hold at least min_cap elements. */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < min_cap) new_cap *= 2;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_cap = new_cap;
}

/* Generate the sequence until it contains at least n elements. */
static void generate_up_to(idx_t n)
{
    while (seq_len < (size_t)n)
    {
        /* Make sure SPF table can index next_k. */
        if ((size_t)next_k >= spf_size) build_spf_to((size_t)next_k * 2 + 10);

        if (qualifies(next_k))
        {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Public entry point – returns the nth term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset safety, tests may ignore this */
    generate_up_to(n);
    return seq_arr[n - 1];
}
