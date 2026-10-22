/*
 * A362987  Lexicographically earliest sequence S of distinct positive terms
 *          such that the successive digits of S are the successive spreads of
 *          S' terms (see Comments for definition of "spread").
 * https://oeis.org/A362987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A362987 – lexicographically earliest distinct positive terms
 *  whose successive digits equal the successive spreads (|first digit –
 *  last digit|) of the terms themselves.
 *
 *  Implementation: greedy construction.  For every required spread r
 *  (0 ≤ r ≤ 9) we keep a minimal candidate number next_scan[r].
 *  When a new term is needed we advance next_scan[r] until an unused
 *  number with the correct spread is found.  All data (sequence,
 *  digit stream, used–set) are statically cached so that the function
 *  works correctly for any order of calls.
 *
 *  The function respects the restrictions of the build environment.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers for digits and spread ---------- */

static inline int first_digit(seq_t x)
{
    int d = (int)x;
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
        d = (int)x;
    }
    return d;
}

static inline int last_digit(seq_t x)
{
    return (int)(x % (seq_t)10);
}

static inline int spread(seq_t x)
{
    int f = first_digit(x);
    int l = last_digit(x);
    return f >= l ? f - l : l - f;
}

/* append the decimal digits of val to the global stream 'digits' */
static void append_digits(seq_t val, unsigned char **digits,
                          size_t *dig_len, size_t *dig_cap)
{
    unsigned char buf[64];
    size_t blen = 0;
    do {
        buf[blen++] = (unsigned char)(val % (seq_t)10);
        val /= (seq_t)10;
    } while (val);
    /* reverse to forward order */
    for (size_t i = 0; i < blen; ++i) {
        if (*dig_len == *dig_cap) {
            *dig_cap <<= 1;
            *digits = (unsigned char *)realloc(*digits, *dig_cap);
        }
        (*digits)[(*dig_len)++] = buf[blen - 1 - i];
    }
}

/* ---------- simple open-addressing hash set ---------- */

#define HASH_INIT_CAP 65536U

typedef struct {
    seq_t *keys;
    bool  *used;
    size_t cap;   /* power of two */
    size_t count;
} HashSet;

static void hash_init(HashSet *h)
{
    h->cap   = HASH_INIT_CAP;
    h->count = 0;
    h->keys  = (seq_t *)calloc(h->cap, sizeof(seq_t));
    h->used  = (bool  *)calloc(h->cap, sizeof(bool  ));
}

static void hash_ensure(HashSet *h)
{
    if (h->count * 3 < h->cap * 2) return;   /* load factor < 0.66 */

    size_t newcap = h->cap << 1;
    seq_t *newk   = (seq_t *)calloc(newcap, sizeof(seq_t));
    bool  *newu   = (bool  *)calloc(newcap, sizeof(bool  ));

    for (size_t i = 0; i < h->cap; ++i) {
        if (h->used[i]) {
            size_t pos = ((size_t)h->keys[i]) & (newcap - 1);
            while (newu[pos]) pos = (pos + 1) & (newcap - 1);
            newu[pos] = true;
            newk[pos] = h->keys[i];
        }
    }
    free(h->keys);
    free(h->used);
    h->keys = newk;
    h->used = newu;
    h->cap  = newcap;
}

static bool hash_find(const HashSet *h, seq_t key)
{
    size_t pos = ((size_t)key) & (h->cap - 1);
    while (h->used[pos]) {
        if (h->keys[pos] == key) return true;
        pos = (pos + 1) & (h->cap - 1);
    }
    return false;
}

static void hash_insert(HashSet *h, seq_t key)
{
    hash_ensure(h);
    size_t pos = ((size_t)key) & (h->cap - 1);
    while (h->used[pos]) {
        if (h->keys[pos] == key) return;
        pos = (pos + 1) & (h->cap - 1);
    }
    h->used[pos] = true;
    h->keys[pos] = key;
    ++h->count;
}

/* ---------- main sequence generator ---------- */

static seq_t get_candidate(int r, HashSet *used, seq_t *next_scan)
{
    seq_t cur = next_scan[r];
    while (true) {
        if (spread(cur) == r && !hash_find(used, cur)) {
            next_scan[r] = cur + 1;
            return cur;
        }
        ++cur;
    }
}

seq_t a(idx_t n)
{
    /* static state -------------------------------------------------------- */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    static unsigned char *dig      = NULL;
    static size_t dig_cap = 0, dig_len = 0;

    static HashSet used_set;
    static bool used_inited = false;

    static seq_t next_scan[10];  /* minimal candidate per spread 0..9 */

    /* initialisation ----------------------------------------------------- */
    if (!used_inited) {
        used_inited = true;
        seq_cap  = 16;
        seq_len  = 0;
        seq      = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        dig_cap = 256;
        dig_len = 0;
        dig     = (unsigned char *)malloc(dig_cap);

        hash_init(&used_set);

        /* initialise minimal candidates for each spread */
        next_scan[0] = 11;   /* 11 -> spread 0 */
        next_scan[1] = 10;   /* 10 -> spread 1 */
        next_scan[2] = 13;   /* first two‑digit number with spread 2 */
        next_scan[3] = 14;
        next_scan[4] = 15;
        next_scan[5] = 16;
        next_scan[6] = 17;
        next_scan[7] = 18;
        next_scan[8] = 19;
        next_scan[9] = 90;   /* 90 -> spread 9 */
    }

    /* build sequence up to index n -------------------------------------- */
    while (seq_len < n) {
        idx_t cur_idx = seq_len + 1;          /* 1‑based */
        seq_t term;
        int required_r;                       /* needed spread for this term */

        if (cur_idx == 1) {
            /* first term: 10 has spread 1 */
            required_r = 1;
            term = get_candidate(required_r, &used_set, next_scan);
        } else {
            required_r = (int)dig[cur_idx - 1];
            term = get_candidate(required_r, &used_set, next_scan);
        }

        /* store term */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = term;

        /* mark as used */
        hash_insert(&used_set, term);

        /* append digits of the term to the global stream */
        append_digits(term, &dig, &dig_len, &dig_cap);
    }

    return seq[n - 1];
}
