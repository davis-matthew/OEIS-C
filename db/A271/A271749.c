/*
 * A271749  Number of set partitions of [n] such that 10 is the largest
 *          element of the last block.
 * https://oeis.org/A271749
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A271749: Number of set partitions of [n] such that 10 is the largest
 * element of the last block.
 *
 * Recurrence (n > 19):
 *   a(n) = 45*a(n-1) - 870*a(n-2) + 9450*a(n-3) - 63273*a(n-4)
 *          + 269325*a(n-5) - 723680*a(n-6) + 1172700*a(n-7)
 *          - 1026576*a(n-8) + 362880*a(n-9)
 *
 * Initial values (offset 10):
 *   a(10)=42294, a(11)=168509, a(12)=724731, a(13)=3321545,
 *   a(14)=16075611, a(15)=81602489, a(16)=432156891,
 *   a(17)=2377526345, a(18)=13540170651, a(19)=79588371929.
 *
 * The function is memoised; it works for arbitrary constructible seq_t
 * (e.g. unsigned __int128 or a big‑integer class). */

#include <stdlib.h>   /* malloc, realloc */

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated capacity of cache */
static idx_t last_computed = 9;       /* largest index that is already cached */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap <= n) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – very unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Base values for n = 10 .. 19.  Stored as unsigned long long to avoid
   truncation at the point of definition; they are cast to seq_t when
   placed into the cache, which causes truncation for small seq_t types,
   but this matches the behaviour of the original definition. */
static const unsigned long long base_vals[10] = {
    42294ULL,     168509ULL,    724731ULL,     3321545ULL,
    16075611ULL,  81602489ULL,  432156891ULL,  2377526345ULL,
    13540170651ULL, 79588371929ULL
};

/* Compute terms up to and including n (inclusive).  The function uses
   the recurrence for n > 19 and copies the first ten base values
   directly for 10 ≤ n ≤ 19.  All intermediate calculations are done
   using seq_t arithmetic, so behaviour is consistent with any seq_t
   implementation. */
static void compute_to(idx_t n)
{
    if (n <= last_computed) return;  /* nothing to do */

    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 19) {
            cache[i] = (seq_t)base_vals[i - 10];
            continue;
        }

        /* Compute the recurrence term. */
        seq_t pos = 0, neg = 0;
        pos += cache[i - 1] * 45;
        neg += cache[i - 2] * 870;
        pos += cache[i - 3] * 9450;
        neg += cache[i - 4] * 63273;
        pos += cache[i - 5] * 269325;
        neg += cache[i - 6] * 723680;
        pos += cache[i - 7] * 1172700;
        neg += cache[i - 8] * 1026576;
        pos += cache[i - 9] * 362880;
        cache[i] = pos - neg;
    }

    last_computed = n;
}

/* Public API: returns a(n) for n ≥ 10, otherwise 0. */
seq_t a(idx_t n)
{
    if (n < 10) return 0;        /* undefined for indices below offset */
    compute_to(n);
    return cache[n];
}
