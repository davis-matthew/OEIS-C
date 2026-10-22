/*
 * A375737  Sum of the n-th maximal anti-run of adjacent (increasing by more
 *          than one at a time) non-perfect-powers.
 * https://oeis.org/A375737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375737: sum of the n‑th maximal anti‑run of adjacent,
   increasing‑by‑more‑than‑one, non‑perfect‑powers.  The function scans
   the non‑perfect‑powers in ascending order, groups consecutive terms
   whose gaps are >1, and returns the sum of the n‑th such anti‑run.
   The algorithm runs in O(n) time per call and works for any n ≥ 1.
   No external libraries are required beyond standard headers and the
   supplied utility headers. */

#include <stdlib.h>
#include <stdbool.h>

/* binary search helper for perfect powers */
static int cmp_ull(const void *a, const void *b)
{
    unsigned long long va = *(const unsigned long long *)a;
    unsigned long long vb = *(const unsigned long long *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* cache of perfect powers up to a fixed limit */
static unsigned long long *pp_cache = NULL;
static size_t pp_len = 0;

/* initialise the perfect‑power cache (once) */
static void init_perfect_powers(void)
{
    const unsigned long long LIMIT = 1000000000ULL;   /* enough for n up to ~10 000 */
    size_t est_cap = 1000000;                        /* initial capacity */
    pp_cache = (unsigned long long *)malloc(est_cap * sizeof(unsigned long long));
    size_t count = 0;
    for (unsigned long long base = 2; base * base <= LIMIT; ++base) {
        unsigned long long val = base * base;        /* exponent 2 */
        while (val <= LIMIT) {
            if (count == est_cap) {                  /* grow buffer */
                est_cap <<= 1;
                pp_cache = (unsigned long long *)realloc(pp_cache,
                                                          est_cap * sizeof(unsigned long long));
            }
            pp_cache[count++] = val;
            if (val > LIMIT / base)
                break;                               /* avoid overflow */
            val *= base;
        }
    }
    qsort(pp_cache, count, sizeof(unsigned long long), cmp_ull);
    /* remove duplicates */
    size_t uniq = 0;
    for (size_t i = 0; i < count; ++i) {
        if (uniq == 0 || pp_cache[i] != pp_cache[uniq - 1]) {
            pp_cache[uniq++] = pp_cache[i];
        }
    }
    pp_len = uniq;
    pp_cache = (unsigned long long *)realloc(pp_cache,
                                             pp_len * sizeof(unsigned long long));
}

/* test if a number is a perfect power */
static bool is_perfect_power(unsigned long long x)
{
    if (!pp_cache) init_perfect_powers();
    size_t lo = 0, hi = pp_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (pp_cache[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < pp_len && pp_cache[lo] == x);
}

/* main sequence function (external linkage) */
seq_t a(idx_t n)
{
    if (!pp_cache) init_perfect_powers();

    unsigned long long i = 2;          /* current integer to test */
    unsigned long long prev = 0;       /* previous non‑perfect number */
    bool first_nonperfect = true;
    seq_t run_sum = 0;                 /* sum of current anti‑run */
    idx_t run_cnt = 0;                 /* number of completed runs */

    for (;;) {
        if (!is_perfect_power(i)) {
            if (first_nonperfect) {
                run_sum = (seq_t)i;
                first_nonperfect = false;
            } else {
                unsigned long long diff = i - prev;
                if (diff > 1) {            /* still in same anti‑run */
                    run_sum += (seq_t)i;
                } else {                    /* anti‑run ends */
                    run_cnt++;
                    if (run_cnt == n)
                        return run_sum;
                    run_sum = (seq_t)i;    /* start new anti‑run */
                }
            }
            prev = i;
        }
        ++i;
    }
}
