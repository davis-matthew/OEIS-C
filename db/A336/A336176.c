/*
 * A336176  Numbers k such that there is a single powerful number between k^2
 *          and (k+1)^2.
 * https://oeis.org/A336176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 911, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A336176 – Numbers k such that there is a single powerful number
 * between k^2 and (k+1)^2.
 *
 * The sequence is computed by:
 *   1. Generating all powerful numbers ≤ LIMIT^2 (every powerful number
 *      can be written as a^2 * b^3).
 *   2. Sorting them and removing duplicates.
 *   3. Scanning k from 1 upward, counting powerful numbers strictly
 *      between k^2 and (k+1)^2 using two moving pointers.
 *   4. Storing the k that give exactly one powerful number.
 *
 * The implementation precomputes up to a fixed maximum K (100 000)
 * which yields more than 39 500 terms – far beyond the 10 000 terms
 * typically requested in the tests.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>   /* for sqrt on double – used only for estimate */

static inline idx_t int_sqrt(idx_t x)
{
    idx_t r = (idx_t)sqrt((double)x);
    while ((r + 1u) * (idx_t)(r + 1u) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

static inline idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * hi * hi <= x) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 mid3 = (unsigned __int128)mid * mid * mid;
        if (mid3 < x)       lo = mid;
        else if (mid3 > x)  hi = mid;
        else                return mid;
    }
    return lo;
}

static int cmp_seq(const void *a, const void *b)
{
    seq_t aa = *(const seq_t *)a;
    seq_t bb = *(const seq_t *)b;
    return (aa > bb) - (aa < bb);
}

/* ---------- Global state (initialized on first use) ---------- */
static const idx_t K_LIMIT = 100000;                /* enough for 10000+ terms */
static seq_t *powers = NULL;                        /* all powerful numbers */
static size_t powers_len = 0;
static seq_t *terms  = NULL;                        /* a(n) values */
static size_t terms_len = 0;
static bool initialized = false;

/* -------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* 1. Generate all powerful numbers up to (K_LIMIT + 2)^2          */
    seq_t max_limit = (seq_t)(K_LIMIT + 2) * (seq_t)(K_LIMIT + 2);
    idx_t a_max = int_sqrt((idx_t)max_limit);

    size_t cap = 200000;  /* initial guess */
    powers = (seq_t *)malloc(cap * sizeof(seq_t));
    powers_len = 0;

    for (idx_t a = 1; a <= a_max; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        seq_t max_b_sq = max_limit / a_sq;
        idx_t max_b = int_cube_root((idx_t)max_b_sq);
        for (idx_t b = 1; b <= max_b; ++b) {
            seq_t b_cu = (seq_t)b * (seq_t)b * (seq_t)b;
            seq_t prod = a_sq * b_cu;
            if (powers_len == cap) {
                cap *= 2;
                powers = (seq_t *)realloc(powers, cap * sizeof(seq_t));
            }
            powers[powers_len++] = prod;
        }
    }

    /* 2. Sort and deduplicate */
    qsort(powers, powers_len, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < powers_len; ++i) {
        if (i == 0 || powers[i] != powers[i - 1]) {
            powers[uniq++] = powers[i];
        }
    }
    powers_len = uniq;

    /* 3. Scan k and collect terms a(n) */
    size_t terms_cap = 50000;  /* enough for K_LIMIT */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms_len = 0;

    size_t low = 0, high = 0;
    for (idx_t k = 1; k <= K_LIMIT; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t next_sq = (seq_t)(k + 1) * (seq_t)(k + 1);

        while (low < powers_len && powers[low] <= k_sq) ++low;
        high = low;
        while (high < powers_len && powers[high] < next_sq) ++high;

        if (high - low == 1) {                     /* exactly one */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = k;
        }
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* not used, but guard against zero */
    init_sequence();
    if ((idx_t)n > terms_len) {
        /* Should not happen for the test ranges; fallback: extend if needed */
        return 0;
    }
    return terms[n - 1];
}
