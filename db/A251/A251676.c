/*
 * A251676  Number of (n+2)X(2+2) 0..2 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 2 or 4.
 * https://oeis.org/A251676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251676: Number of (n+2)×4 0..2 arrays avoiding 3×3 subblocks
 *  whose row, column, diagonal or antidiagonal sum to 2 or 4.
 *
 *  The implementation uses a dynamic‑programming approach.  For every
 *  pair of consecutive rows (r1,r2) in the array it keeps the number
 *  of valid prefixes that end with that pair.  Adding a new row r3
 *  is possible only if all 3×3 subblocks that involve r1,r2,r3 are
 *  allowed.  The adjacency lists for the 81×81 possible pairs are
 *  pre‑computed once, and each DP step updates the counts for the
 *  next row.  The total for a given n is the sum of the counts over
 *  all pairs after n‑1 extensions (the first two rows are unrestricted).
 *
 *  For a C++ build (seq_t can be an arbitrary‑precision class such as
 *  boost::multiprecision::cpp_int) the DP and the result cache are
 *  kept in std::vector<seq_t> so that constructors/destructors are
 *  called correctly.  For a C build (seq_t is a POD integer type) the
 *  same data are held in malloc‑allocated arrays.
 *
 *  The algorithm runs in O(81² × n) time and requires only a few
 *  kilobytes of memory for the adjacency tables, plus O(81²) values
 *  for the DP and a simple cache for the results.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <vector>
#endif

/* --------------------------------------------------------------- */
/*  Static tables and helper data                                    */
/* --------------------------------------------------------------- */

static bool init_done = false;

/* Enumeration of the 81 possible rows (4 cells, digits 0..2) */
static unsigned char row_digits[81][4];

/* For every pair (r1,r2) the list of next rows r3 that keep all
 * subblocks legal.  next_row[id][k] gives the k‑th valid r3, and
 * next_cnt[id] is the number of such rows.  The pair id is r1*81+r2.
 */
static unsigned char next_row[81 * 81][81];
static unsigned char next_cnt[81 * 81];
static unsigned char pair_r2[81 * 81];

/* DP arrays: counts for the current and next step.  In C++ they
 * are std::vector<seq_t>; in C they are malloc'd blocks of seq_t.
 */
#if defined(__cplusplus)
static std::vector<seq_t> dp_vec;
static std::vector<seq_t> dp_next_vec;
#else
static seq_t *dp = NULL;
static seq_t *dp_next = NULL;
#endif

/* Result cache: a(n) values already computed. */
#if defined(__cplusplus)
static std::vector<seq_t> cache_vec;     /* index 0 unused */
#else
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;        /* actual number of stored elements, index 0 unused */
#endif

/* Current maximum n that has been cached. */
static idx_t computed_n = 0;

/* --------------------------------------------------------------- */
/*  Core function                                                    */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
     *  One‑time global initialization
     * ----------------------------------------------------------------
     */
    if (!init_done)
    {
        /* Enumerate rows */
        for (int r = 0; r < 81; ++r) {
            int tmp = r;
            for (int c = 3; c >= 0; --c) {
                row_digits[r][c] = (unsigned char)(tmp % 3);
                tmp /= 3;
            }
        }

        /* Build adjacency lists */
        for (int r1 = 0; r1 < 81; ++r1) {
            for (int r2 = 0; r2 < 81; ++r2) {
                int id = r1 * 81 + r2;
                pair_r2[id] = (unsigned char)r2;
                unsigned char cnt = 0;

                for (int r3 = 0; r3 < 81; ++r3) {
                    bool ok = true;
                    const unsigned char *a = row_digits[r1];
                    const unsigned char *b = row_digits[r2];
                    const unsigned char *c = row_digits[r3];
                    int s;

                    /* left 3×3 block (cols 0,1,2) */
                    s = a[0] + a[1] + a[2];
                    if (s == 2 || s == 4) ok = false;
                    if (ok) { s = b[0] + b[1] + b[2]; if (s == 2 || s == 4) ok = false; }
                    if (ok) { s = c[0] + c[1] + c[2]; if (s == 2 || s == 4) ok = false; }
                    if (ok) {
                        s = a[0] + b[0] + c[0];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[1] + b[1] + c[1];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[2] + b[2] + c[2];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[0] + b[1] + c[2];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[2] + b[1] + c[0];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (!ok) continue;

                    /* right 3×3 block (cols 1,2,3) */
                    s = a[1] + a[2] + a[3];
                    if (s == 2 || s == 4) ok = false;
                    if (ok) { s = b[1] + b[2] + b[3]; if (s == 2 || s == 4) ok = false; }
                    if (ok) { s = c[1] + c[2] + c[3]; if (s == 2 || s == 4) ok = false; }
                    if (ok) {
                        s = a[1] + b[1] + c[1];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[2] + b[2] + c[2];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[3] + b[3] + c[3];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[1] + b[2] + c[3];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (ok) {
                        s = a[3] + b[2] + c[1];
                        if (s == 2 || s == 4) ok = false;
                    }
                    if (!ok) continue;

                    /* valid transition */
                    next_row[id][cnt++] = (unsigned char)r3;
                }
                next_cnt[id] = cnt;
            }
        }

        /* Allocate DP arrays */
#if defined(__cplusplus)
        dp_vec.assign(81 * 81, 1);          /* two unrestricted rows */
        dp_next_vec.assign(81 * 81, 0);
#else
        dp = (seq_t*)malloc(81 * 81 * sizeof(seq_t));
        dp_next = (seq_t*)malloc(81 * 81 * sizeof(seq_t));
        for (size_t i = 0; i < 81 * 81; ++i) dp[i] = 1;
        for (size_t i = 0; i < 81 * 81; ++i) dp_next[i] = 0;
#endif

        /* Allocate result cache */
#if defined(__cplusplus)
        cache_vec.push_back(0);          /* index 0 unused */
#else
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                    /* unused */
        cache_len = 1;                   /* only the dummy element */
#endif
        computed_n = 0;
        init_done = true;
    }

    /* ----------------------------------------------------------------
     *  Extend DP until the requested n is available
     * ----------------------------------------------------------------
     */
    while (computed_n < n)
    {
        /* Zero dp_next */
#if defined(__cplusplus)
        for (size_t i = 0; i < 81 * 81; ++i) dp_next_vec[i] = 0;
#else
        for (size_t i = 0; i < 81 * 81; ++i) dp_next[i] = 0;
#endif

        /* Transition */
        for (size_t i = 0; i < 81 * 81; ++i)
        {
            seq_t cnt;
#if defined(__cplusplus)
            cnt = dp_vec[i];
#else
            cnt = dp[i];
#endif
            if (cnt == 0) continue;

            unsigned char r2 = pair_r2[i];
            unsigned char *list = next_row[i];
            unsigned char len = next_cnt[i];

            for (unsigned char k = 0; k < len; ++k)
            {
                unsigned char r3 = list[k];
                unsigned int next = r2 * 81 + r3;
#if defined(__cplusplus)
                dp_next_vec[next] += cnt;
#else
                dp_next[next] += cnt;
#endif
            }
        }

        /* Swap dp and dp_next */
#if defined(__cplusplus)
        std::swap(dp_vec, dp_next_vec);
#else
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
#endif

        /* Sum total counts for this n */
        seq_t total = 0;
#if defined(__cplusplus)
        for (size_t i = 0; i < 81 * 81; ++i) total += dp_vec[i];
#else
        for (size_t i = 0; i < 81 * 81; ++i) total += dp[i];
#endif

        /* Store in cache */
        ++computed_n;
#if defined(__cplusplus)
        if (computed_n >= cache_vec.size())
            cache_vec.resize(computed_n + 1);
        cache_vec[computed_n] = total;
#else
        if (computed_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[computed_n] = total;
        cache_len = computed_n + 1;
#endif
    }

    /* Return the cached value */
#if defined(__cplusplus)
    return cache_vec[n];
#else
    return cache[n];
#endif
}
