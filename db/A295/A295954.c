/*
 * A295954  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + 1, where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n) + 1 Initial values: a(0)=2,
 *            a(1)=4, b(0)=1, b(1)=3, b(2)=5 (a(n)) and (b(n)) are increasing
 *            complementary sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static arrays to memoise a() and b(), plus a bitmap of used b-values */
static seq_t *aArr   = NULL;
static size_t aCap   = 0;
static idx_t  aSize  = (idx_t)-1;   /* last index filled, (idx_t)-1 means empty */

static seq_t *bArr   = NULL;
static size_t bCap   = 0;
static idx_t  bSize  = (idx_t)-1;

static bool *used    = NULL;
static size_t usedCap = 0;

/* Ensure aArr has room for at least 'need' elements */
static void ensure_a_cap(size_t need) {
    if (need <= aCap) return;
    size_t newCap = aCap ? aCap << 1 : 8;
    while (newCap < need) newCap <<= 1;
    seq_t *tmp = (seq_t*)realloc(aArr, newCap * sizeof(seq_t));
    if (tmp == NULL) abort();
    aArr = tmp;
    aCap = newCap;
}

/* Ensure bArr has room for at least 'need' elements */
static void ensure_b_cap(size_t need) {
    if (need <= bCap) return;
    size_t newCap = bCap ? bCap << 1 : 8;
    while (newCap < need) newCap <<= 1;
    seq_t *tmp = (seq_t*)realloc(bArr, newCap * sizeof(seq_t));
    if (tmp == NULL) abort();
    bArr = tmp;
    bCap = newCap;
}

/* Ensure used[] has room for indices up to at least 'need-1' */
static void ensure_used_cap(size_t need) {
    if (need <= usedCap) return;
    size_t newCap = usedCap ? usedCap << 1 : 8;
    while (newCap < need) newCap <<= 1;
    bool *tmp = (bool*)realloc(used, newCap * sizeof(bool));
    if (tmp == NULL) abort();
    for (size_t i = usedCap; i < newCap; ++i) tmp[i] = false;
    used = tmp;
    usedCap = newCap;
}

/* Binary search whether x occurs in the currently computed aArr[0..aSize] */
static bool a_contains(seq_t x) {
    if (aSize == (idx_t)-1) return false;
    idx_t lo = 0, hi = aSize;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (aArr[mid] == x) return true;
        if (aArr[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n) {
    /* one‑time initialisation */
    static bool inited = false;
    static idx_t next_b_candidate = 1;   /* first integer to try for new b(k) */

    if (!inited) {
        /* initialise aArr with a(0)..a(2) */
        ensure_a_cap(3);
        aArr[0] = (seq_t)2;
        aArr[1] = (seq_t)4;
        aArr[2] = aArr[1] + aArr[0] + (seq_t)5 + 1;   /* b(2)=5 */
        aSize  = 2;   /* last index now filled */

        /* initialise bArr with b(0)..b(2) */
        ensure_b_cap(3);
        bArr[0] = (seq_t)1;
        bArr[1] = (seq_t)3;
        bArr[2] = (seq_t)5;
        bSize  = 2;

        /* bitmap of used b-values */
        ensure_used_cap(6);            /* indices 0..5 needed */
        used[1] = true;
        used[3] = true;
        used[5] = true;

        inited = true;
    }

    /* trivial cases */
    if (n == 0) return aArr[0];
    if (n == 1) return aArr[1];
    if (n == 2) return aArr[2];

    /* extend arrays up to required index n */
    for (idx_t k = aSize + 1; k <= n; ++k) {
        seq_t b_val;

        if (k <= 2) {           /* already initialised */
            b_val = bArr[k];
        } else {
            /* find the smallest unused integer for b(k) */
            while (true) {
                ensure_used_cap((size_t)next_b_candidate + 1);
                if (used[(size_t)next_b_candidate]) {
                    ++next_b_candidate;
                    continue;
                }
                if (a_contains((seq_t)next_b_candidate)) {
                    ++next_b_candidate;
                    continue;
                }
                break;
            }
            b_val = (seq_t)next_b_candidate;
            /* store b(k) and mark it as used */
            ensure_b_cap(k + 1);
            bArr[k] = b_val;
            ++next_b_candidate;
            used[(size_t)(next_b_candidate - 1)] = true;  /* mark the used value */
        }

        /* compute a(k) */
        ensure_a_cap(k + 1);
        aArr[k] = aArr[k - 1] + aArr[k - 2] + b_val + 1;
        aSize = k;
    }

    return aArr[n];
}
