/*
 * A383691  Square numbers with distinct digits from 1-9 that have an initial
 *          string of two or more digits forming a square number.
 * https://oeis.org/A383691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383691: squares whose decimal digits are all distinct 1‑9
   and which contain a proper prefix (length ≥2) that is itself a square.
   This implementation generates terms by brute‑force scanning of integer
   squares up to 987 654 321, caching results for subsequent calls. */ 

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* --- Configuration ----------------------------------------------------- */
#define MAX_ROOT 31426ULL          /* floor(sqrt(987654321)) */

/* --- Cache for already found terms ------------------------------------ */
static seq_t *cache = NULL;        /* array of found squares          */
static size_t cache_len = 0;       /* number of terms stored          */
static size_t cache_cap = 0;       /* allocated capacity of cache     */
static idx_t last_root = 0;        /* last root processed             */

/* Ensure the cache has capacity for at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t grow = cache_cap ? cache_cap * 2 : 16;
    while (grow < need)
        grow *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, grow * sizeof(seq_t));
    if (tmp == NULL) {
        /* Hard failure – the sequence cannot continue. */
        exit(1);
    }
    cache = tmp;
    cache_cap = grow;
}

/* Test whether a 64‑bit integer is a perfect square. */
static bool is_square_uint64(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((double)v);
    while (r * r > v)
        --r;
    while ((r + 1) * (r + 1) <= v)
        ++r;
    return r * r == v;
}

/* Check whether a square number satisfies the sequence conditions. */
static bool valid_square(seq_t sq)
{
    /* Convert sq to decimal digits in an array (least significant first). */
    uint32_t buf[10];          /* max 10 digits – 9 needed for this sequence */
    int len = 0;
    seq_t tmp = sq;
    bool seen[10] = { false };

    if (tmp == 0)
        return false;

    while (tmp > 0) {
        uint32_t d = (uint32_t)(tmp % 10);
        if (d == 0 || seen[d])
            return false;      /* zero digit or duplicate */
        seen[d] = true;
        buf[len++] = d;
        tmp /= 10;
    }

    if (len < 2)                /* need at least two digits */
        return false;

    /* Build prefixes from most significant digit to test if any is a square. */
    uint64_t prefix = 0;
    for (int i = len - 1; i >= 1; --i) {   /* loop over all prefixes of length ≥2 */
        prefix = prefix * 10 + buf[i];
        if (i <= len - 2) { /* prefix length from 2 to len-1 */
            if (is_square_uint64(prefix))
                return true;
        }
    }
    return false;
}

/* --- Sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for offset 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (last_root >= MAX_ROOT)   /* no larger squares possible */
            break;
        ++last_root;
        seq_t rt = (seq_t)last_root;
        seq_t sq = rt * rt;          /* square of the root */

        if (valid_square(sq)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = sq;
        }
    }

    if (n <= cache_len)
        return cache[n - 1];
    return 0;                        /* insufficient terms found */
}
