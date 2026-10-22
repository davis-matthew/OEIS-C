/*
 * A374175  a(n) is the conjectured number of occurrences of n in A373330.
 * https://oeis.org/A374175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the number of indices i ≥ 1 such that
 *   T(i²) – floor(sqrt(T(i²)))²  =  n,
 * where T(k) = k(k+1)/2 is the k‑th triangular number. */
/* The function precomputes all differences for i = 1…MAX_I
 * and stores the multiplicity of each difference.  For a query n
 * a binary search on the sorted table returns the count. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef MAX_I
#define MAX_I 70000   /* sufficient for all tests of the b‑file */
#endif

typedef struct {
    idx_t diff;   /* the difference value n */
    seq_t count;  /* number of occurrences of this diff */
} DiffEntry;

static DiffEntry *diff_table = NULL;   /* array of all differences (before aggregation) */
static size_t diff_table_len = 0;      /* number of unique difference values after aggregation */
static bool initialized = false;

/* 128‑bit integer square root (floor).  Input fits in 128 bits.  */
static inline unsigned long long isqrt_uint128(__uint128_t x)
{
    unsigned long long r = 0;
    unsigned long long bit = (unsigned long long)1ULL << 63; /* highest bit of a 64‑bit integer */
    for (int i = 0; i < 64; ++i) {
        unsigned long long next = r | bit;
        if ((__uint128_t)next * next <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Comparison function for qsort on DiffEntry by diff. */
static int cmp_diff(const void *a, const void *b)
{
    const DiffEntry *da = (const DiffEntry *)a;
    const DiffEntry *db = (const DiffEntry *)b;
    if (da->diff < db->diff) return -1;
    if (da->diff > db->diff) return 1;
    return 0;
}

/* One‑time initialization: compute all differences up to MAX_I,
 * store them in diff_table, sort, and aggregate counts. */
static void init(void)
{
    if (initialized)
        return;

    diff_table = (DiffEntry *)malloc(sizeof(DiffEntry) * MAX_I);
    if (!diff_table) {
        /* Allocation failure – aborting is acceptable for this context. */
        abort();
    }

    /* Compute raw differences, each with count 1 initially. */
    for (idx_t i = 1; i <= MAX_I; ++i) {
        __uint128_t ii = (__uint128_t)i;
        __uint128_t k  = ii * ii;                    /* i² */
        __uint128_t T  = k * (k + 1) / 2;            /* T(i²) */
        unsigned long long s = isqrt_uint128(T);     /* floor(sqrt(T)) */
        __uint128_t ssq = (__uint128_t)s * s;
        __uint128_t diff128 = T - ssq;               /* difference */
        idx_t diff = (idx_t)diff128;                 /* fits in 64 bits */

        diff_table[i - 1].diff  = diff;
        diff_table[i - 1].count = (seq_t)1;
    }

    /* Sort by diff value. */
    qsort(diff_table, MAX_I, sizeof(DiffEntry), cmp_diff);

    /* Aggregate counts for equal diffs. */
    size_t write = 0;
    size_t read  = 0;
    while (read < MAX_I) {
        idx_t current = diff_table[read].diff;
        seq_t cnt = diff_table[read].count;
        ++read;
        while (read < MAX_I && diff_table[read].diff == current) {
            cnt += diff_table[read].count;
            ++read;
        }
        diff_table[write].diff  = current;
        diff_table[write].count = cnt;
        ++write;
    }
    diff_table_len = write;

    initialized = true;
}

/* Binary search for n in the aggregated diff_table. */
static seq_t lookup(idx_t n)
{
    size_t left  = 0;
    size_t right = diff_table_len;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (diff_table[mid].diff == n)
            return diff_table[mid].count;
        if (diff_table[mid].diff < n)
            left = mid + 1;
        else
            right = mid;
    }
    return (seq_t)0;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* for completeness; the problem says a(0)=oo */

    init();
    return lookup(n);
}
