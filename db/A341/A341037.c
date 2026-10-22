/*
 * A341037  The successive remainders of the division by (a(n) + a(n+1)) of
 *          concat(a(n),a(n+1)) rebuild the sequence itself.
 * https://oeis.org/A341037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 24, u32 n <= 29, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_len, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined recursively.
   a(1) = 1.
   For n > 1, a(n) is the smallest unused positive integer y such that
   concat(a(n‑1),y) mod (a(n‑1)+y) = a(n‑1).

   Here concat(a,b) = a·10^d + b where d = digit_len(b). */

/* Required public function */
seq_t a(idx_t n);

/* Local helpers – all marked static so there is a single instance per
   shared library or program. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_len.h"   /* seq_t digit_len(seq_t x);  */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base,
                                   uint64_t exp, seq_t mod); */

/* ---------- cache for computed terms ---------- */
static seq_t *seq_cache      = NULL;   /* array of a(1)...a(k) */
static size_t seq_size       = 0;      /* number of elements stored */
static size_t seq_capacity   = 0;      /* allocated capacity */

/* ---------- bitmap for small integers ---------- */
static bool *used_small      = NULL;   /* true iff value <= MAX_SMALL used */
#define MAX_SMALL  2000000U         /* arbitrary but covers early terms */

/* ---------- local utility functions ---------- */

/* Ensure seq_cache has at least 'needed' slots.  Returns 0 on failure. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= seq_capacity)
        return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* OOM – terminate */
    seq_cache = tmp;
    seq_capacity = new_cap;
}

/* Initialise the cache on the very first call. */
static void init_cache(void)
{
    if (seq_cache) return;        /* already initialized */

    seq_capacity = 1;
    seq_cache = (seq_t *)malloc(sizeof(seq_t));
    if (!seq_cache) abort();
    seq_size = 1;
    seq_cache[0] = (seq_t)1;      /* a(1) = 1 */

    used_small = (bool *)malloc((MAX_SMALL + 1) * sizeof(bool));
    if (!used_small) abort();
    memset(used_small, 0, (MAX_SMALL + 1) * sizeof(bool));
    used_small[1] = true;          /* 1 is now used */
}

/* Return true if 'val' has already appeared in the sequence so far. */
static bool used(seq_t val)
{
    if (val <= (seq_t)MAX_SMALL && used_small[(uint64_t)val])
        return true;

    for (size_t i = 0; i < seq_size; ++i)
        if (seq_cache[i] == val)
            return true;
    return false;
}

/* Find the next term after 'prev' according to the definition. */
static seq_t next_term(seq_t prev)
{
    seq_t candidate = (seq_t)1;      /* start searching from 1 */

    for (;;) {
        if (used(candidate))
            { candidate++; continue; }

        /* Check the condition:
           (prev * 10^d + candidate) % (prev + candidate) == prev,
           where d = digit_len(candidate). */
        seq_t mod = prev + candidate;
        unsigned d = digit_len(candidate);
        seq_t pow10 = mod_pow((seq_t)10, (uint64_t)d, mod);
        seq_t lhs = (prev * pow10 + candidate) % mod;

        if (lhs == prev)
            return candidate;   /* condition satisfied */

        candidate++;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1 – undefined for 0; return 0 as a safe value */
        return (seq_t)0;

    init_cache();

    /* Extend the cache up to the requested index if necessary. */
    while (seq_size < (size_t)n) {
        seq_t next = next_term(seq_cache[seq_size - 1]);

        ensure_cache_capacity(seq_size + 1);
        seq_cache[seq_size++] = next;
        if (next <= (seq_t)MAX_SMALL)
            used_small[(uint64_t)next] = true;
    }

    return seq_cache[n - 1];
}
