/*
 * A305983  Filter sequence combining from all proper divisors d of n, the
 *          prime signature of 2d+1.
 * https://oeis.org/A305983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the restricted‑growth sequence index of the * multiset
 *            of prime signatures of 2d+1 for all proper divisors d of n
 * Range:     u8 n <= 845, u16 n <= 9924, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* External types supplied by the build system. */
extern seq_t a(idx_t n);

/* ----------  Helper data structures  ---------- */

struct Vec {
    int *data;   /* exponents of a single 2d+1 */
    int len;     /* number of exponents (always >0) */
};

struct Signature {
    int clen;    /* flattened length of the key array */
    int *data;   /* key array: len followed by exponents, repeated for each vector */
};

/* ----------  Global caches  ---------- */

static seq_t *cache   = NULL;        /* cached a(n) values */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static struct Signature *sig_arr = NULL; /* array of distinct signatures */
static size_t sig_cap  = 0;
static size_t sig_cnt  = 0;

static uint32_t *spf   = NULL;        /* smallest‑prime‑factor sieve */
static size_t   spf_sz = 0;          /* current size of spf array (limit+1) */

/* ----------  Utility functions  ---------- */

static int cmp_int(const void *a, const void *b)
{
    int aa = *(const int *)a;
    int bb = *(const int *)b;
    return aa - bb;
}

static int cmp_vec(const void *a, const void *b)
{
    const struct Vec *va = (const struct Vec *)a;
    const struct Vec *vb = (const struct Vec *)b;
    int i = 0;
    while (i < va->len && i < vb->len) {
        if (va->data[i] != vb->data[i])
            return va->data[i] - vb->data[i];
        ++i;
    }
    return va->len - vb->len;
}

/* Ensure the SPF sieve covers all numbers up to 'limit' (inclusive). */
static void ensure_spf(size_t limit)
{
    if (spf == NULL) {
        spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (spf == NULL) exit(1);
    } else if (spf_sz > limit) {
        return;
    } else {
        spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
        if (spf == NULL) exit(1);
    }

    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_sz = limit + 1;
}

/* Compute a(n) for a given n.  Assumes spf is ready. */
static seq_t compute_value(idx_t n)
{
    const size_t MAX_DIVS = 256;
    struct Vec vecs[MAX_DIVS];
    size_t veccnt = 0;

    uint64_t tn = (uint64_t)n;

    for (idx_t i = 1; (uint64_t)i * i <= tn; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* process d1 */
            if (d1 < n) {
                uint64_t m = 2 * d1 + 1;
                int ex[16];
                int expcnt = 0;
                while (m > 1) {
                    uint32_t p = spf[(size_t)m];
                    int cnt = 0;
                    while (m % p == 0) { m /= p; ++cnt; }
                    ex[expcnt++] = cnt;
                }
                if (expcnt > 1) qsort(ex, expcnt, sizeof(int), cmp_int);
                int *data = (int *)malloc(expcnt * sizeof(int));
                memcpy(data, ex, expcnt * sizeof(int));
                vecs[veccnt].data = data;
                vecs[veccnt].len = expcnt;
                ++veccnt;
            }

            /* process d2 if distinct */
            if (d2 != d1 && d2 < n) {
                uint64_t m = 2 * d2 + 1;
                int ex[16];
                int expcnt = 0;
                while (m > 1) {
                    uint32_t p = spf[(size_t)m];
                    int cnt = 0;
                    while (m % p == 0) { m /= p; ++cnt; }
                    ex[expcnt++] = cnt;
                }
                if (expcnt > 1) qsort(ex, expcnt, sizeof(int), cmp_int);
                int *data = (int *)malloc(expcnt * sizeof(int));
                memcpy(data, ex, expcnt * sizeof(int));
                vecs[veccnt].data = data;
                vecs[veccnt].len = expcnt;
                ++veccnt;
            }
        }
    }

    /* Sort the vectors lexicographically to make the representation canonical. */
    if (veccnt > 1) qsort(vecs, veccnt, sizeof(struct Vec), cmp_vec);

    /* Build the flattened key array. */
    int clen = 0;
    for (size_t i = 0; i < veccnt; ++i)
        clen += 1 + vecs[i].len;

    int *key = NULL;
    if (clen > 0) {
        key = (int *)malloc(clen * sizeof(int));
        int offset = 0;
        for (size_t i = 0; i < veccnt; ++i) {
            key[offset++] = vecs[i].len;
            memcpy(key + offset, vecs[i].data, vecs[i].len * sizeof(int));
            offset += vecs[i].len;
        }
    }

    /* Compare with existing signatures. */
    for (size_t i = 0; i < sig_cnt; ++i) {
        if (sig_arr[i].clen != clen) continue;
        if (clen == 0 || memcmp(sig_arr[i].data, key, clen * sizeof(int)) == 0)
            return (seq_t)(i + 1);
    }

    /* If new, store the signature. */
    if (sig_cnt == sig_cap) {
        size_t new_cap = sig_cap ? sig_cap << 1 : 8;
        struct Signature *tmp = (struct Signature *)realloc(sig_arr, new_cap * sizeof(struct Signature));
        if (tmp == NULL) exit(1);
        sig_arr = tmp;
        sig_cap = new_cap;
    }

    int *copy = NULL;
    if (clen > 0) {
        copy = (int *)malloc(clen * sizeof(int));
        memcpy(copy, key, clen * sizeof(int));
    }
    sig_arr[sig_cnt].clen = clen;
    sig_arr[sig_cnt].data = copy;
    ++sig_cnt;

    /* Free temporary structures. */
    for (size_t i = 0; i < veccnt; ++i) free(vecs[i].data);
    if (key) free(key);

    return (seq_t)sig_cnt;
}

/* ----------  Public API  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    /* Lazily initialise cache. */
    if (cache_len == 0) {
        ensure_spf(20005);           /* most probable initial need */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(1);
    }

    /* Extend cache and compute missing values. */
    if (cache_len < (size_t)n) {
        /* Ensure spf is large enough for all future values we will compute. */
        ensure_spf(2 * (size_t)n + 1);

        size_t required = (size_t)n;
        if (cache_cap < required) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_len + 1; i <= required; ++i) {
            cache[i - 1] = compute_value((idx_t)i);
        }
        cache_len = required;
    }

    return cache[n - 1];
}
