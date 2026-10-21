/*
 * A296282  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)*b(n) with a(0)=2, a(1)=4,
 *            b(0)=1, b(1)=3, b(2)=5 and the sequences a() and b() are the
 *            complementary increasing integer sequences
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static bool inited = false;
    static seq_t *a_cache = NULL;      /* a values */
    static seq_t *b_cache = NULL;      /* b values */
    static size_t a_cap = 0, b_cap = 0; /* capacities */
    static idx_t  max_computed = 0;    /* highest index computed */
    static seq_t  next_candidate = (seq_t)6; /* next integer to try for b */

    /* ----- initialisation ----- */
    if (!inited) {
        /* allocate space for at least three terms and the requested n */
        size_t need = (size_t)(n + 1);
        if (need < 3) need = 3;
        size_t cap = 16;
        while (cap < need) cap <<= 1;
        a_cap = b_cap = cap;

        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!a_cache || !b_cache) exit(EXIT_FAILURE);

        /* initial terms */
        a_cache[0] = (seq_t)2;
        a_cache[1] = (seq_t)4;
        b_cache[0] = (seq_t)1;
        b_cache[1] = (seq_t)3;
        b_cache[2] = (seq_t)5;

        /* compute a(2) */
        seq_t prod = b_cache[0] * b_cache[1] * b_cache[2];
        a_cache[2] = a_cache[1] + a_cache[0] + prod;

        max_computed = 2;          /* indices 0,1,2 known */
        inited = true;
    }

    /* ----- use cached value if already available ----- */
    if (n <= max_computed)
        return a_cache[n];

    /* ----- grow allocation if necessary ----- */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cap = new_cap;
    }
    if ((size_t)(n + 1) > b_cap) {
        size_t new_cap = b_cap;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        b_cache = tmp;
        b_cap = new_cap;
    }

    /* ----- extend the sequences up to the requested index ----- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* find next b(i) not appearing in a_cache[0..max_computed] */
        seq_t cand = next_candidate;
        for (;;) {
            idx_t lo = 0, hi = max_computed;
            bool present = false;
            while (lo <= hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                if (a_cache[mid] == cand) {
                    present = true;
                    break;
                }
                if (a_cache[mid] < cand)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }
            if (!present) break;      /* cand is free */
            ++cand;                    /* try next integer */
        }
        b_cache[i] = cand;
        next_candidate = cand + 1;    /* advance for next use */

        /* compute a(i) */
        seq_t prod = b_cache[i-2] * b_cache[i-1] * b_cache[i];
        a_cache[i] = a_cache[i-1] + a_cache[i-2] + prod;
    }

    max_computed = n;
    return a_cache[n];
}
