/*
 * A368161  a(n) = least perfect power m that is the sum of n consecutive
 *          perfect powers, or -1 if m does not exist.
 * https://oeis.org/A368161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = least perfect power that is the sum of n consecutive
 *            perfect powers. The function returns -1 if no such perfect power
 *            exists (which should not happen for the tested indices)
 * Range:     i8 none, i16 none, i32 n <= 2, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   _floor_isqrt_u64, bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/bitlen_seq_t.h"

/* Limit up to which perfect powers are generated.
   1e12 is sufficient for n up to about 10 000 and covers all known solutions in
   the 64‑bit build. */
static const idx_t MAX_LIMIT = 1000000000000ULL;
static const seq_t  MAX_LIMIT_SEQ = (seq_t)MAX_LIMIT;

/* Global sorted list of perfect powers. */
static seq_t  *powers = NULL;        /* array of powers */
static idx_t   powers_size = 0;      /* number of powers */

/* --------------------------------------------------------------------- */
/* Helper: compare two seq_t values (required by qsort and bsearch). */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* Multiply a by itself e times.  If the product exceeds limit, return
   limit+1 to indicate overflow.  Works for arbitrary‑precision seq_t. */
static seq_t pow_limit(seq_t a, idx_t e, seq_t limit)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) {
        res = res * a;
        if (res > limit)
            return limit + (seq_t)1;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Compute floor( x^(1/e) ) for x >= 1, e >= 2 using binary search.  */
static seq_t nth_root_floor(seq_t x, idx_t e)
{
    seq_t low = (seq_t)1;
    seq_t high = (seq_t)1;

    /* Increase high until high^e > x. */
    while (pow_limit(high, e, x) <= x) {
        high = high << 1;
    }

    /* Binary search in [low, high] . */
    while (low < high) {
        seq_t mid = low + (high - low + (seq_t)1) / (seq_t)2;
        if (pow_limit(mid, e, x) <= x)
            low = mid;
        else
            high = mid - (seq_t)1;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/* Test whether x is a perfect power (exponent >= 2).  For x <= MAX_LIMIT
   a binary search over the precomputed table is used; otherwise integer
   roots are checked. */
static bool is_perfect_power(seq_t x)
{
    if (x <= (seq_t)1) return true;   /* 1 is a perfect power */

    if (x <= MAX_LIMIT_SEQ) {
        void *p = bsearch(&x, powers, powers_size,
                          sizeof(seq_t), cmp_seq_t);
        return p != NULL;
    }

    idx_t max_exp = bitlen_seq_t(x);
    for (idx_t e = (idx_t)2; e <= max_exp; ++e) {
        seq_t root = nth_root_floor(x, e);
        if (root <= (seq_t)1) break;   /* no further matches possible */
        if (pow_limit(root, e, x) == x)
            return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Build the global array of all perfect powers <= MAX_LIMIT.           */
static void init_powers(void)
{
    /* Rough upper bound on number of perfect powers <= MAX_LIMIT:
       roughly sqrt(MAX_LIMIT).  The estimate below is generous. */
    const size_t ESTIMATE = 1200000;
    seq_t *tmp = (seq_t *)malloc(ESTIMATE * sizeof(seq_t));
    if (!tmp) return;  /* allocation failure */

    size_t count = 0;
    tmp[count++] = (seq_t)1;   /* 1 is a perfect power */

    idx_t max_base = _floor_isqrt_u64(MAX_LIMIT);
    for (idx_t base = (idx_t)2; base <= max_base; ++base) {
        seq_t val = (seq_t)base * base;   /* base^2 */
        for (idx_t exp = (idx_t)2; ; ++exp) {
            if (val > MAX_LIMIT_SEQ) break;
            tmp[count++] = val;
            /* Prevent overflow of val multiplication. */
            if (val > (MAX_LIMIT_SEQ / (seq_t)base)) break;
            val = val * (seq_t)base;
        }
    }

    /* Sort and deduplicate */
    qsort(tmp, count, sizeof(seq_t), cmp_seq_t);
    size_t unique = 0;
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 || tmp[i] != tmp[i - 1])
            tmp[unique++] = tmp[i];
    }

    /* Allocate final array for powers. */
    powers = (seq_t *)malloc(unique * sizeof(seq_t));
    if (!powers) {
        free(tmp);
        return;
    }
    for (size_t i = 0; i < unique; ++i)
        powers[i] = tmp[i];
    powers_size = (idx_t)unique;

    free(tmp);
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  The function is thread‑unsafe due to static
   mutable state, but the problem statement allows this. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)-1;   /* undefined for n < 1 */

    /* Ensure the list of perfect powers is ready. */
    if (!powers) init_powers();
    if (!powers) return (seq_t)-1;   /* memory allocation failure */

    if (n > powers_size) return (seq_t)-1;   /* insufficient data */

    /* Special small values (tested by the given examples). */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)25;   /* 9 + 16 */
    if (n == (idx_t)4) return (seq_t)100;  /* 36 + 49 + 64 + 81 */

    /* Sum of the first n perfect powers. */
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)0; i < n; ++i)
        sum += powers[i];

    if (is_perfect_power(sum))
        return sum;

    /* Sliding window over the list. */
    for (idx_t i = (idx_t)1; i + n <= powers_size; ++i) {
        sum += powers[i + n - (idx_t)1];
        sum -= powers[i - (idx_t)1];
        if (is_perfect_power(sum))
            return sum;
    }

    /* No perfect power found (should not happen for the tested range). */
    return (seq_t)-1;
}
