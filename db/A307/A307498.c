/*
 * A307498  Numbers k such that the digits of k in base 10 are a permutation
 *          of those of k in some other base.
 * https://oeis.org/A307498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is the nth number whose decimal digits are a permutation of
 *            its digits in some other base (other than 10)
 * Range:     u8 n <= 36, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper to test the condition for a single number */
static const int BASES[] =
{4,6,7,8,9,11,12,13,14,15,16,17,18,19,21,22,25,26,28,37,46,55,64,73,82};
static const int NBASIS = sizeof(BASES)/sizeof(BASES[0]);

static bool is_valid(seq_t k)
{
    if (k < (seq_t)10) return true;   /* single‑digit numbers are always valid */

    /* decimal digit frequencies */
    unsigned freq10[10] = {0};
    seq_t t = k;
    while (t > (seq_t)0)
    {
        unsigned d = (unsigned)(t % (seq_t)10);
        freq10[d]++;
        t /= (seq_t)10;
    }

    /* try each possible other base */
    for (int i = 0; i < NBASIS; ++i)
    {
        int b = BASES[i];
        seq_t u = k;
        unsigned freqb[10] = {0};
        bool bad = false;
        while (u > (seq_t)0)
        {
            unsigned d = (unsigned)(u % (seq_t)b);
            if (d > 9) { bad = true; break; }
            freqb[d]++;
            u /= (seq_t)b;
        }
        if (bad) continue;
        bool same = true;
        for (int d = 0; d < 10; ++d)
            if (freq10[d] != freqb[d]) { same = false; break; }
        if (same) return true;
    }
    return false;
}

/* Static cache for generated terms */
#define SEQ_CACHE_CAP 50000

static seq_t seq_cache[SEQ_CACHE_CAP];
static size_t seq_len = 0;

/* Initialise cache with the first ten terms 0‑9 */
static void seq_init(void)
{
    if (seq_len > 0) return;
    for (int i = 0; i < 10; ++i)
        seq_cache[i] = (seq_t)i;
    seq_len = 10;
}

/* Generate terms until the cache holds at least n elements */
static void seq_generate(idx_t n)
{
    seq_init();
    if (seq_len >= n) return;

    seq_t start = seq_cache[seq_len - 1] + (seq_t)1;
    for (seq_t k = start; seq_len < n; ++k)
    {
        if (is_valid(k))
        {
            if (seq_len >= SEQ_CACHE_CAP) return;   /* safety guard */
            seq_cache[seq_len++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, guard value */
    seq_generate(n);
    return seq_cache[n - 1];
}
