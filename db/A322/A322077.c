/*
 * A322077  In the ranked poset of integer partitions ordered by refinement,
 *          number of integer partitions coarser (greater) than or equal to
 *          the integer partition whose multiplicities are the prime indices
 *          of n in weakly decreasing order.
 * https://oeis.org/A322077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of distinct coarser partitions of the partition
 *            λ(n), where λ(n) has multiplicities equal to the prime indices
 *            of n in weakly decreasing order. The algorithm enumerates all
 *            set partitions of the multiset of parts and counts the distinct
 *            multiset of block sums
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   lsb_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/lsb_index.h"

/* ---------- data structures ---------- */
typedef struct {
    unsigned char len;            /* number of sums in this multiset */
    unsigned char sums[16];       /* sums are small (≤ 91) */
} Key;

typedef struct {
    size_t size; /* number of keys stored */
    size_t cap;  /* allocated capacity */
    Key *data;
} Result;

/* ---------- global buffers used by the recursive routine ---------- */
static int N_global;                 /* number of elements in the multiset of parts */
static int *sum_for_mask_global;      /* sum of values for every mask (size = 1<<N_global) */
static Result **memo_global;          /* memoization table for each mask */

static void add_key(Result *res, const Key *k)
{
    for (size_t i = 0; i < res->size; ++i) {
        if (res->data[i].len == k->len &&
            memcmp(res->data[i].sums, k->sums, k->len) == 0)
            return; /* duplicate */
    }
    if (res->size == res->cap) {
        size_t newcap = (res->cap == 0) ? 4 : res->cap * 2;
        Key *newdata = (Key *)realloc(res->data, newcap * sizeof(Key));
        if (!newdata) exit(1);
        res->data = newdata;
        res->cap = newcap;
    }
    res->data[res->size++] = *k;
}

static Key merge_key(const Key *rest, unsigned char sumSub)
{
    Key r;
    r.len = rest->len + 1;
    int pos = 0;
    while (pos < rest->len && sumSub > rest->sums[pos]) pos++;
    for (int i = 0; i < pos; ++i) r.sums[i] = rest->sums[i];
    r.sums[pos] = sumSub;
    for (int i = pos; i < rest->len; ++i) r.sums[i + 1] = rest->sums[i];
    return r;
}

/* ---------- recursive enumeration of partitions for a given mask ---------- */
static Result* computeSet(int mask)
{
    if (memo_global[mask]) return memo_global[mask];

    Result *res = (Result *)malloc(sizeof(Result));
    if (!res) exit(1);
    res->size = 0;
    res->cap = 0;
    res->data = NULL;

    if (mask == 0) {
        Key empty;
        empty.len = 0;
        add_key(res, &empty);
        memo_global[mask] = res;
        return res;
    }

    int firstbit = mask & -mask;             /* lowest set bit */
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        if (!(sub & firstbit)) continue;     /* canonical ordering */
        int sumSub   = sum_for_mask_global[sub];
        int restMask = mask ^ sub;
        Result *restRes = computeSet(restMask);
        for (size_t i = 0; i < restRes->size; ++i) {
            Key merged = merge_key(&restRes->data[i], (unsigned char)sumSub);
            add_key(res, &merged);
        }
    }

    memo_global[mask] = res;
    return res;
}

/* ---------- prime utilities ---------- */
#define MAX_PRIME 10000
static int primes[MAX_PRIME];
static int prime_index[MAX_PRIME + 1];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    bool is_composite[MAX_PRIME + 1] = {0};
    prime_cnt = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            prime_index[i] = prime_cnt;  /* 1‑based index */
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_inited = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!primes_inited) init_primes();

    /* ----- factor n and build list of prime indices (multiplicities) ----- */
    idx_t temp = n;
    int indices[32];
    size_t idx_len = 0;

    for (int pi = 0; pi < prime_cnt &&
              (idx_t)primes[pi] * (idx_t)primes[pi] <= temp; ++pi) {
        int p = primes[pi];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            int pidx = prime_index[p];   /* 1‑based index of prime p */
            while (exp--) indices[idx_len++] = pidx;
        }
    }
    if (temp > 1) {                     /* remaining prime factor > sqrt(n) */
        int pidx = prime_index[(int)temp];
        indices[idx_len++] = pidx;
    }

    /* sort indices descending (weakly decreasing) */
    for (size_t i = 0; i < idx_len; ++i)
        for (size_t j = i + 1; j < idx_len; ++j)
            if (indices[j] > indices[i]) {
                int t = indices[i];
                indices[i] = indices[j];
                indices[j] = t;
            }

    /* ----- build multiset of part values ----- */
    int values[32];
    N_global = 0;
    for (size_t j = 0; j < idx_len; ++j) {
        int mult = indices[j];
        int val = (int)(j + 1);          /* part size */
        while (mult--) {
            values[N_global++] = val;
        }
    }

    /* ----- precompute sum for each mask ----- */
    int mask_cnt = 1 << N_global;
    sum_for_mask_global = (int *)calloc(mask_cnt, sizeof(int));
    if (!sum_for_mask_global) exit(1);
    for (int mask = 1; mask < mask_cnt; ++mask) {
        int lowbit = mask & -mask;
        int pos = lsb_index(lowbit);
        sum_for_mask_global[mask] = sum_for_mask_global[mask ^ lowbit] + values[pos];
    }

    /* ----- recursive enumeration of set partitions ----- */
    memo_global = (Result **)calloc(mask_cnt, sizeof(Result *));
    if (!memo_global) exit(1);

    Result *finalRes = computeSet(mask_cnt - 1);
    seq_t answer = (seq_t)finalRes->size;  /* number of distinct coarser partitions */

    /* ----- cleanup ----- */
    for (int m = 0; m < mask_cnt; ++m) {
        Result *r = memo_global[m];
        if (!r) continue;
        free(r->data);
        free(r);
    }
    free(memo_global);
    free(sum_for_mask_global);

    return answer;
}
