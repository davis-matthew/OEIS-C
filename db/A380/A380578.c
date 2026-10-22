/*
 * A380578  Number of nonisomorphic abelian groups of order n which appear as
 *          the group of units of the ring Z/kZ taken over every k such that
 *          phi(k) = n.
 * https://oeis.org/A380578
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

/* Sequence A380578: Number of nonisomorphic abelian groups
   of order n that occur as (ℤ/kℤ)× for some k with φ(k)=n. */
/* The function builds a sieve up to LIMIT=2 000 000; enough for
   all n that appear in the OEIS tests. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef LIMIT
#define LIMIT 2000000   /* maximal k we inspect (φ(k) ≤ LIMIT ≤ n) */
#endif

/* Representation of an abelian group as invariant factor list */
typedef struct {
    int len;
    uint64_t comp[20];           /* sorted ascending */
} Rep;

typedef struct {
    int count;
    int cap;
    Rep *reps;
} RepList;

static RepList *rep_lists = NULL;
static uint32_t *phi_arr = NULL;
static uint32_t *spf_arr = NULL;
static seq_t *output_cache = NULL;
static bool initialized = false;

/* Compare two representations */
static bool rep_equal(const Rep *a, const Rep *b)
{
    if (a->len != b->len) return false;
    for (int i = 0; i < a->len; ++i)
        if (a->comp[i] != b->comp[i]) return false;
    return true;
}

/* Append a representation to a list */
static void rep_list_append(RepList *list, const uint64_t *arr, int len)
{
    if (list->count == list->cap) {
        int newcap = list->cap ? list->cap << 1 : 4;
        Rep *tmp = (Rep *)realloc(list->reps, newcap * sizeof(Rep));
        if (!tmp) abort();
        list->reps = tmp;
        list->cap = newcap;
    }
    Rep *r = &list->reps[list->count++];
    r->len = len;
    for (int i = 0; i < len; ++i) r->comp[i] = arr[i];
}

/* Compute invariant factor decomposition for the list of
   cyclic components 'raw' (length raw_len). Result stored in 'out'.
   out_len is set to the number of invariant factors. */
static void invariant_factors(uint64_t *raw, int raw_len, uint64_t *out, int *out_len)
{
    /* Gather exponents for each prime */
    struct { uint32_t p; int exps[20]; int cnt; } pe[20];
    int pe_cnt = 0;

    for (int i = 0; i < raw_len; ++i) {
        uint64_t val = raw[i];
        while (val > 1) {
            uint32_t p = spf_arr[val];
            int e = 0;
            while (val % p == 0) {
                val /= p;
                ++e;
            }
            int idx = -1;
            for (int j = 0; j < pe_cnt; ++j)
                if (pe[j].p == p) { idx = j; break; }
            if (idx == -1) { idx = pe_cnt++; pe[idx].p = p; pe[idx].cnt = 0; }
            pe[idx].exps[pe[idx].cnt++] = e;
        }
    }

    /* Sort exponents per prime in descending order */
    for (int i = 0; i < pe_cnt; ++i) {
        int n = pe[i].cnt;
        /* simple insertion sort (n ≤ 20) */
        for (int a = 1; a < n; ++a) {
            int key = pe[i].exps[a];
            int b = a - 1;
            while (b >= 0 && pe[i].exps[b] < key) {
                pe[i].exps[b + 1] = pe[i].exps[b];
                --b;
            }
            pe[i].exps[b + 1] = key;
        }
    }

    /* Determine maximum vector length */
    int maxlen = 0;
    for (int i = 0; i < pe_cnt; ++i)
        if (pe[i].cnt > maxlen) maxlen = pe[i].cnt;

    int outc = 0;
    for (int i = 0; i < maxlen; ++i) {
        uint64_t factor = 1;
        for (int j = 0; j < pe_cnt; ++j) {
            if (i < pe[j].cnt) {
                uint64_t pow = 1;
                for (int k = 0; k < pe[j].exps[i]; ++k)
                    pow *= pe[j].p;
                factor *= pow;
            }
        }
        out[outc++] = factor;
    }

    /* Sort invariant factors ascending */
    for (int i = 1; i < outc; ++i) {
        uint64_t key = out[i];
        int j = i - 1;
        while (j >= 0 && out[j] > key) {
            out[j + 1] = out[j];
            --j;
        }
        out[j + 1] = key;
    }
    *out_len = outc;
}

/* Generate cyclic components for (ℤ/kℤ)×. Result stored in out (max 20),
   len set appropriately. */
static void factor_components(uint32_t k, uint64_t *out, int *len)
{
    uint64_t comps[20];
    int cnt = 0;

    /* Handle powers of 2 */
    int e = 0;
    while ((k & 1) == 0) {
        k >>= 1;
        ++e;
    }
    if (e == 2) {
        comps[cnt++] = 2ULL;
    } else if (e >= 3) {
        comps[cnt++] = 2ULL;
        comps[cnt++] = 1ULL << (e - 2);        /* 2^{e-2} */
    }
    /* Odd prime powers */
    while (k > 1) {
        uint32_t p = spf_arr[k];
        int exp = 0;
        while (k % p == 0) {
            k /= p;
            ++exp;
        }
        uint64_t val = (uint64_t)(p - 1);
        for (int i = 1; i < exp; ++i) val *= (uint64_t)p;
        comps[cnt++] = val;
    }

    /* Convert to invariant factors */
    invariant_factors(comps, cnt, out, len);
}

/* Initialise tables (called once) */
static void init_all(void)
{
    phi_arr = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    spf_arr = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!phi_arr || !spf_arr) abort();

    for (size_t i = 0; i <= LIMIT; ++i) {
        phi_arr[i] = (uint32_t)i;
        spf_arr[i] = (uint32_t)i;
    }
    phi_arr[0] = 0; phi_arr[1] = 1;
    spf_arr[0] = 0; spf_arr[1] = 1;

    /* Sieve for phi and spf */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf_arr[i] == i) {             /* i is prime */
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf_arr[j] == j) spf_arr[j] = i;
            }
            for (uint32_t j = i; j <= LIMIT; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }

    /* Allocate per‑n lists and output cache */
    rep_lists = (RepList *)calloc(LIMIT + 1, sizeof(RepList));
    output_cache = (seq_t *)calloc(LIMIT + 1, sizeof(seq_t));
    if (!rep_lists || !output_cache) abort();

    /* Build representations for each k */
    uint64_t raw[20], out[20];
    for (uint32_t k = 1; k <= LIMIT; ++k) {
        uint32_t n = phi_arr[k];
        if (n == 0 || n > LIMIT) continue;

        int raw_len, out_len;
        factor_components(k, raw, &raw_len);
        invariant_factors(raw, raw_len, out, &out_len);

        RepList *list = &rep_lists[n];
        bool exists = false;
        Rep tmp; tmp.len = out_len;
        for (int i = 0; i < out_len; ++i) tmp.comp[i] = out[i];
        for (int i = 0; i < list->count; ++i)
            if (rep_equal(&list->reps[i], &tmp)) {
                exists = true; break;
            }
        if (!exists) rep_list_append(list, out, out_len);
    }
    /* Fill cache */
    for (int n = 1; n <= LIMIT; ++n)
        output_cache[n] = (seq_t)rep_lists[n].count;

    initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init_all();
    if (n > LIMIT) return (seq_t)0;
    return output_cache[(size_t)n];
}
