/*
 * A369333  Positive integers m such that there exist distinct pairs (a,b) and
 *          (c,d) with a <= b, c <= d, and m = a*b*(c+d) = (a+b)*c*d.
 * https://oeis.org/A369333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd, pack_pair, seq_cmp, unpack_a, unpack_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * (A369333) Positive integers m such that there exist distinct pairs
 * (a,b) and (c,d), a ≤ b, c ≤ d,
 * with a*b*(c+d) = (a+b)*c*d   (equivalently 1/a+1/b = 1/c+1/d).
 *
 * This implementation enumerates pairs (a,b) in increasing order,
 * groups them by the reduced rational r = (a+b)/(a*b), and for each
 * new pair combines it with all earlier pairs in the same group to
 * produce candidate m.  All candidates are collected, deduplicated,
 * and merged into a global sorted list.  The list is memoised and
 * extended until the requested index is available.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_cmp.h"
#include "../utils/pack_pair.h"
#include "../utils/unpack_a.h"
#include "../utils/unpack_b.h"

/* Hash table entry for a reduced rational (num/den) */
struct FractionEntry {
    uint64_t num;          /* reduced numerator (a+b) */
    uint64_t den;          /* reduced denominator (a*b) */
    uint64_t *pairs;       /* packed (a,b) as ((uint64_t)a << 32) | b */
    size_t len;            /* number of stored pairs */
    size_t cap;            /* capacity of pairs array */
};

static struct FractionEntry *hash_table = NULL;
static const size_t HASH_SIZE = (1 << 18);     /* 262144 – power of two */

static seq_t *m_values = NULL;   /* sorted unique sequence values */
static size_t m_count = 0;       /* number of values stored */

/* initialise the hash table */
static void init_hash_table(void)
{
    hash_table = (struct FractionEntry*)calloc(HASH_SIZE, sizeof(struct FractionEntry));
}

/* simple multiplicative hash for 64‑bit integers */
static inline size_t hash_uint64(uint64_t x)
{
    return (size_t)((x * 11400714819323198485ull) >> (64 - 18));
}

/* lookup or create entry for reduced rational (num,den) */
static struct FractionEntry* get_or_create_entry(uint64_t num, uint64_t den)
{
    size_t idx = hash_uint64(num ^ (den << 1));
    while (true) {
        struct FractionEntry *e = &hash_table[idx];
        if (e->den == 0) {                 /* empty slot */
            e->num = num;
            e->den = den;
            e->len = 0;
            e->cap = 4;
            e->pairs = (uint64_t*)malloc(e->cap * sizeof(uint64_t));
            return e;
        }
        if (e->num == num && e->den == den) {
            return e;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
}

/* add a pair (a,b) to an entry's list */
static void add_pair_entry(struct FractionEntry *e, uint32_t a, uint32_t b)
{
    if (e->len >= e->cap) {
        e->cap <<= 1;
        e->pairs = (uint64_t*)realloc(e->pairs, e->cap * sizeof(uint64_t));
    }
    e->pairs[e->len++] = pack_pair(a, b);
}

/* temporary buffer for new candidate values */
struct TempBuffer {
    seq_t *data;
    size_t len;
    size_t cap;
};
static void buffer_init(struct TempBuffer *buf, size_t cap)
{
    buf->data = (seq_t*)malloc(cap * sizeof(seq_t));
    buf->len  = 0;
    buf->cap  = cap;
}
static void buffer_push(struct TempBuffer *buf, seq_t val)
{
    if (buf->len >= buf->cap) {
        buf->cap = buf->cap ? buf->cap << 1 : 16;
        buf->data = (seq_t*)realloc(buf->data, buf->cap * sizeof(seq_t));
    }
    buf->data[buf->len++] = val;
}
static void buffer_free(struct TempBuffer *buf)
{
    free(buf->data);
}

/* merge a sorted unique list of new values into the global sequence */
static void merge_new_values(seq_t *new_vals, size_t new_len)
{
    size_t i = 0, j = 0, k = 0;
    size_t new_total = m_count + new_len;
    seq_t *merged = (seq_t*)malloc(new_total * sizeof(seq_t));

    while (i < m_count && j < new_len) {
        int cmp = seq_cmp(&m_values[i], &new_vals[j]);
        if (cmp < 0) {
            merged[k++] = m_values[i++];
        } else if (cmp > 0) {
            merged[k++] = new_vals[j++];
        } else {
            merged[k++] = m_values[i++];
            j++;                /* skip duplicate */
        }
    }
    while (i < m_count) merged[k++] = m_values[i++];
    while (j < new_len) merged[k++] = new_vals[j++];

    free(m_values);
    m_values = merged;
    m_count  = k;
}

/* generate sequence values until at least ‘need’ terms are available */
static void ensure_generated(idx_t need)
{
    if (need <= m_count) return;               /* already enough */

    uint32_t limit    = 0;                     /* current maximum value of a or b */
    uint32_t extend   = 200;                  /* step size to increase limit */

    while (m_count < need) {
        uint32_t new_limit = limit + extend;
        struct TempBuffer buf;
        buffer_init(&buf, 1024);               /* initial capacity for candidates */

        /* enumerate all new pairs (a,b) with a ≤ b, up to new_limit */
        for (uint32_t a = 1; a <= new_limit; ++a) {
            uint32_t b_start = (a > limit) ? a : (limit + 1);
            for (uint32_t b = b_start; b <= new_limit; ++b) {
                uint64_t num = (uint64_t)a + (uint64_t)b;
                uint64_t den = (uint64_t)a * (uint64_t)b;
                seq_t g = gcd((seq_t)num, (seq_t)den);
                num /= (uint64_t)g;
                den /= (uint64_t)g;
                struct FractionEntry *e = get_or_create_entry(num, den);

                /* combine with every earlier pair in the same group */
                for (size_t p = 0; p < e->len; ++p) {
                    uint32_t ca = unpack_a(e->pairs[p]);
                    uint32_t cb = unpack_b(e->pairs[p]);
                    seq_t m = (seq_t)a * (seq_t)b * (seq_t)(ca + cb);
                    buffer_push(&buf, m);
                }
                add_pair_entry(e, a, b);
            }
        }

        /* sort the newly found candidates and remove duplicates */
        if (buf.len > 0) {
            qsort(buf.data, buf.len, sizeof(seq_t), seq_cmp);
            size_t uniq = 1;
            for (size_t i = 1; i < buf.len; ++i)
                if (seq_cmp(&buf.data[uniq-1], &buf.data[i]) != 0)
                    buf.data[uniq++] = buf.data[i];

            merge_new_values(buf.data, uniq);
        }
        buffer_free(&buf);
        limit = new_limit;
        if (extend < 5000) extend += 200;
    }
}

/* public function required by the task */
seq_t a(idx_t n)
{
    if (hash_table == NULL) init_hash_table();
    if (n == 0) return (seq_t)0;
    ensure_generated(n);
    return (n <= m_count) ? m_values[n - 1] : (seq_t)0;
}
