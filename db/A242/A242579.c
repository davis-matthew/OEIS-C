/*
 * A242579  Number of length n+3 0..3 arrays with no four elements in a row
 *          with pattern aabb (with a!=b) and new values 0..3 introduced in
 *          0..3 order.
 * https://oeis.org/A242579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence * * a(n) = 7*a(n-1) - 18*a(n-2) + 32*a(n-3)
 *            * - 45*a(n-4) + 37*a(n-5) - 21*a(n-6) + 9*a(n-7) * * Initial
 *            values (offset 1): * a(1)=14, a(2)=45, a(3)=159, a(4)=587,
 *            a(5)=2209, * a(6)=8391, a(7)=32020 * * The implementation below
 *            does NOT use the empirical recurrence; * it coun...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static state for memoisation */
static seq_t *cur  = NULL;        /* counts for current length  */
static seq_t *next = NULL;        /* temporary counts */
static idx_t  cur_len = 3;        /* length represented by cur   */
static idx_t  max_computed_n = 0; /* highest n already computed   */

static seq_t *values = NULL;      /* array of computed a(n)      */
static size_t values_cap = 0;     /* allocated capacity of values */
static size_t values_len = 0;     /* number of values stored (≠cap) */

/* helper: ensure values array large enough for index 'needed' */
static void ensure_values_capacity(idx_t needed)
{
    if (needed < values_cap) return;
    size_t newcap = values_cap ? values_cap : 16;
    while (newcap <= (size_t)needed) newcap <<= 1;
    seq_t *newptr = (seq_t *)calloc(newcap, sizeof(seq_t));
    if (values) {
        memcpy(newptr, values, values_len * sizeof(seq_t));
        free(values);
    }
    values = newptr;
    values_cap = newcap;
}

/* initialise DP state for sequences of length 3 */
static void init(void)
{
    cur  = (seq_t *)calloc(256, sizeof(seq_t));
    next = (seq_t *)calloc(256, sizeof(seq_t));

    cur_len = 3;
    max_computed_n = 0;

    /* allocate values array with one dummy slot (index 0 unused) */
    values_cap = 16;
    values = (seq_t *)calloc(values_cap, sizeof(seq_t));
    values_len = 0;             /* no a(n) computed yet */

    /* generate all length‑3 sequences obeying the introduction rule */
    /* first element must be 0 */
    int a = 0;
    for (int b = 0; b <= 1; ++b) {
        int max1 = (b > a) ? b : a;      /* max value after first two elements */
        /* third element can be any existing value or the next new value */
        for (int c = 0; c <= max1; ++c) {
            int max2 = (c > max1) ? c : max1;
            size_t id = (size_t)max2 * 64 + ((size_t)(a * 4 + b) * 4 + c);
            cur[id] += (seq_t)1;
        }
        if (max1 < 3) {
            int c = max1 + 1;
            int max2 = c;   /* max1 < 3 guarantees c ≤ 3 */
            size_t id = (size_t)max2 * 64 + ((size_t)(a * 4 + b) * 4 + c);
            cur[id] += (seq_t)1;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* below the offset — behaviour undefined, but return 0 */

    static int initialized = 0;
    if (!initialized) {
        init();
        initialized = 1;
    }

    /* ensure we have stored a(n) */
    ensure_values_capacity(n);

    /* extend DP until we have a(n) */
    while (max_computed_n < n) {
        /* reset 'next' array */
        memset(next, 0, 256 * sizeof(seq_t));

        /* transition from cur to next */
        for (idx_t id = 0; id < 256; ++id) {
            seq_t ways = cur[id];
            if (!ways) continue;

            idx_t maxVal = id / 64;
            idx_t rest   = id % 64;
            int  t0 = rest / 16;
            int  t1 = (rest / 4) % 4;
            int  t2 = rest % 4;

            /* try all allowed new values */
            for (int x = 0; x <= (int)maxVal; ++x) {
                /* skip forbidden pattern a a b b */
                if (t0 == t1 && t2 == x && t0 != t2) continue;
                idx_t new_id = maxVal * 64 + ((t1 * 4 + t2) * 4 + x);
                next[new_id] += ways;
            }
            if (maxVal < 3) {           /* we may introduce a new value */
                int x = (int)maxVal + 1;
                if (!(t0 == t1 && t2 == x && t0 != t2)) {
                    idx_t new_id = (maxVal + 1) * 64 +
                                   ((t1 * 4 + t2) * 4 + x);
                    next[new_id] += ways;
                }
            }
        }

        /* swap arrays */
        seq_t *tmp = cur; cur = next; next = tmp;
        cur_len++;

        /* sum over all states to get a(n) for this length */
        seq_t sum = 0;
        for (idx_t id = 0; id < 256; ++id) sum += cur[id];

        /* store in values array */
        values_len++;
        values[values_len] = sum;
        max_computed_n = values_len;
    }

    return values[n];
}
