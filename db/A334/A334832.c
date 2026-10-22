/*
 * A334832  Numbers whose squarefree part is congruent to 1 (mod 24).
 * https://oeis.org/A334832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer whose square‑free part ≡ 1 (mod
 *            24)
 * Range:     u8 n <= 21, u16 n <= 4110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Choose a static limit large enough for the required n.
   For n up to about 1e4 the values lie far below 2·10⁶,
   so a limit of two million guarantees enough terms. */
#define LIMIT 2000000

static idx_t *spf = NULL;          /* smallest prime factor array */
static seq_t *seq_arr = NULL;      /* array of sequence values */
static size_t seq_cap = 0;         /* capacity of seq_arr */
static size_t seq_len = 0;         /* number of filled elements */
static bool initialized = false;  /* init flag */

/* Simple linear sieve for smallest prime factors */
static void build_spf(idx_t limit)
{
    spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 2; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
}

/* Compute the square‑free part of x using the pre‑computed spf array. */
static seq_t squarefree_part(idx_t x)
{
    seq_t sf = 1;
    while (x > 1)
    {
        idx_t p = spf[x];
        int exp = 0;
        do
        {
            x /= p;
            ++exp;
        } while (x % p == 0);
        if (exp & 1)          /* odd exponent → include the prime once */
            sf *= (seq_t)p;
    }
    return sf;
}

/* Build the full sequence up to the chosen LIMIT. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    build_spf(LIMIT);

    /* Rough estimate: about 1/24 of numbers qualify, so we use a buffer. */
    seq_cap = LIMIT / 2;                  /* 1 million entries */
    seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    for (idx_t num = 1; num <= LIMIT; ++num)
    {
        seq_t sf = squarefree_part(num);
        if (sf % 24 == 1)
        {
            if (seq_len == seq_cap)
            {
                /* very unlikely, but be safe */
                seq_cap *= 2;
                seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = num;
        }
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of OEIS range, but harmless */
    init_sequence();
    /* The sequence is already sorted; use 0‑based indexing. */
    return seq_arr[(size_t)(n - 1)];
}
