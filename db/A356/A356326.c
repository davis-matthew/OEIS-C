/*
 * A356326  The terms in the negaFibonacci representation of a(n) are the
 *          terms in common in the negaFibonacci representations of n and -n.
 * https://oeis.org/A356326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the integer whose negaFibonacci representation uses
 *            exactly the terms common to the negaFibonacci representations of
 *            n and –n. Index 0 corresponds to n = 0, index 1 to n = 1, etc
 * Range:     i8 n <= 0, i16 n <= 8984, i32 n <= 8984, i64 n <= 8984,
 *            i128 n <= 8984, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* maximal index needed for representing |n| <= 10000
   F(21) = 10946 is already sufficient                */
#define NEGAFIB_MAX_IDX 21

/* representation entries: sum(=long long) and mask (bits 0..20)
   no consecutive bits satisfied automatically during generation   */
struct rep_entry {
    long long sum;
    uint32_t mask;
};

static struct rep_entry entries[30000];   /* enough for F(23)=28657 */
static size_t entry_count = 0;
static int initialized = 0;
static seq_t negafib[NEGAFIB_MAX_IDX + 1];   /* 1‑based */

/* comparison function for qsort on sum */
static int cmp_entries(const void *a, const void *b)
{
    long long sa = ((const struct rep_entry *)a)->sum;
    long long sb = ((const struct rep_entry *)b)->sum;
    if (sa < sb) return -1;
    return (sa > sb) ? 1 : 0;
}

/* binary search for a given key; returns mask or 0 if not found */
static uint32_t lookup_mask(long long key)
{
    size_t lo = 0, hi = entry_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        long long midsum = entries[mid].sum;
        if (midsum == key) return entries[mid].mask;
        if (midsum < key) lo = mid + 1;
        else hi = mid;
    }
    return 0;   /* not found – should not happen for |n| <= 10000 */
}

/* precompute negafibonacci values, all legitimate masks,
   and their sums, then sort the entries */
static void init(void)
{
    if (initialized) return;
    /* Fibonacci numbers needed to build negafibs */
    seq_t fib[NEGAFIB_MAX_IDX + 2];
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i <= NEGAFIB_MAX_IDX + 1; ++i) fib[i] = fib[i-1] + fib[i-2];

    /* negafibonacci: F(-k) = (-1)^(k+1) * F(k)   */
    for (int k = 1; k <= NEGAFIB_MAX_IDX; ++k) {
        seq_t val = fib[k];
        if ((k & 1) == 0) val = -val;   /* even k -> negative */
        negafib[k] = val;
    }

    /* enumerate masks with no consecutive 1's */
    uint32_t max_mask = 1u << NEGAFIB_MAX_IDX;
    for (uint32_t mask = 0; mask < max_mask; ++mask) {
        if ((mask & (mask << 1)) != 0) continue;   /* consecutive bits forbidden */
        long long s = 0;
        for (int i = 1; i <= NEGAFIB_MAX_IDX; ++i)
            if (mask & (1u << (i-1)))
                s += (long long)negafib[i];
        /* store entry; will be sorted later */
        entries[entry_count].sum = s;
        entries[entry_count].mask = mask;
        ++entry_count;
    }

    /* sort by sum so binary search can be used */
    qsort(entries, entry_count, sizeof(struct rep_entry), cmp_entries);
    initialized = 1;
}

/* public function implementing A356326 */
seq_t a(idx_t n)
{
    init();   /* ensure everything precomputed only once */

    long long key_n  = (long long)n;
    long long key_mn = -((long long)n);

    uint32_t mask_n  = lookup_mask(key_n);
    uint32_t mask_mn = lookup_mask(key_mn);

    uint32_t common = mask_n & mask_mn;

    seq_t result = 0;
    for (int i = 1; i <= NEGAFIB_MAX_IDX; ++i)
        if (common & (1u << (i-1)))
            result += negafib[i];

    return result;
}
