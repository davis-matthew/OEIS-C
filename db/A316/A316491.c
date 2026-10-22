/*
 * A316491  Number of ways to represent 8*n + 4 as the sum of four distinct
 *          odd squares.
 * https://oeis.org/A316491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to represent 8*n + 4 as the sum of * a
 *            quadruple of distinct odd squares
 * Range:     u8 n <= 1731, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---- integer square root (floor) for 64‑bit unsigned ---------------- */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---- pair of indices (i<j) and sum of their squares ---------------- */
struct Pair
{
    int a;   /* smaller index */
    int b;   /* larger index */
    int sum; /* sum of squares of the two odd numbers */
};

/* maximum possible number of odd numbers up to sqrt(8*10000+4) */
#define MAX_ODD   142               /* (283+1)/2 */
#define MAX_PAIR  ((MAX_ODD*(MAX_ODD-1))/2)

static struct Pair pairs[MAX_PAIR];
static int odd[MAX_ODD];
static int sq[MAX_ODD];

/* ---- helper: lower bound on pairs with sum == target ----------------- */
static size_t lower_bound_pair(const struct Pair *arr, size_t n, int target)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid].sum < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---- helper: upper bound on pairs with sum == target ----------------- */
static size_t upper_bound_pair(const struct Pair *arr, size_t n, int target)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid].sum <= target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* target value 8*n + 4 */
    const idx_t T = (8 * n) + 4;

    /* largest odd <= sqrt(T) */
    idx_t s = floor_isqrt_u64(T);
    if (s < 1)
        return 0;
    if ((s & 1) == 0)
        --s; /* make odd */
    if (s < 1)
        return 0;

    /* number of odd integers <= s (1,3,5,...) */
    idx_t oddcnt = (s / 2) + 1;
    if (oddcnt < 4)
        return 0; /* need at least four distinct odds */

    /* Build odd numbers and their squares */
    for (idx_t i = 0; i < oddcnt; ++i) {
        odd[i] = 1 + (int)(2 * i);
        sq[i]  = odd[i] * odd[i];
    }

    /* Build all unordered pairs of distinct odd numbers */
    size_t paircnt = 0;
    for (idx_t i = 0; i < oddcnt - 1; ++i) {
        for (idx_t j = i + 1; j < oddcnt; ++j) {
            pairs[paircnt].a   = (int)i;
            pairs[paircnt].b   = (int)j;
            pairs[paircnt].sum = sq[i] + sq[j];
            ++paircnt;
        }
    }

    /* Sort pairs by their sum (ascending) */
    /* simple insertion sort is fine for <=10k elements */
    for (size_t i = 1; i < paircnt; ++i) {
        struct Pair key = pairs[i];
        size_t j = i;
        while (j > 0 && pairs[j - 1].sum > key.sum) {
            pairs[j] = pairs[j - 1];
            --j;
        }
        pairs[j] = key;
    }

    seq_t count = 0;

    /* Count unordered quadruples of distinct odd squares */
    for (size_t i = 0; i < paircnt; ++i) {
        int target = (int)T - pairs[i].sum;
        if (target < pairs[i].sum)   /* ensure i<j in terms of pair sums */
            continue;

        size_t l = lower_bound_pair(pairs, paircnt, target);
        size_t r = upper_bound_pair(pairs, paircnt, target);
        if (l >= r)
            continue;

        for (size_t j = l; j < r; ++j) {
            if (j <= i)
                continue;          /* avoid double counting pairs */

            /* enforce canonical order: pair i contains the two
             * smallest indices of the quadruple                */
            int a1 = pairs[i].a, b1 = pairs[i].b;
            int a2 = pairs[j].a, b2 = pairs[j].b;
            if (!(a1 < a2 && b1 < a2))
                continue;

            /* Indices are distinct by construction */
            ++count;
        }
    }

    return count;
}
