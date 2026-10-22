/*
 * A329934  a(1)=1, a(2)=1, a(n) = (number of times a(n-1) has appeared
 *          before) + (number of times a(n-2) has appeared before).
 * https://oeis.org/A329934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1; a(n) = (freq of a(n-1)) + (freq of a(n-2))
 *            for n>2
 * Range:     u8 n <= 4000, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------------------------- static state --------------------------------- */
static seq_t *seq_cache = NULL;      /* 1‑based sequence cache */
static idx_t  seq_cap   = 0;          /* allocated capacity (seq_cache)   */
static idx_t  seq_used  = 0;          /* highest index already computed */

struct freq_entry {                  /* simple linear‑scan hash table */
    seq_t val;
    seq_t count;
};

static struct freq_entry *freq_tab = NULL;   /* frequency table */
static size_t            freq_cap = 0;        /* allocated capacity   */
static size_t            freq_len = 0;        /* number of entries    */

/* --------------------------------- helpers ---------------------------------- */
static void ensure_seq_cap(idx_t n) {
    if (seq_cap < n + 1) {                         /* need room for indices 1..n */
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

static void ensure_freq_cap(void) {
    if (freq_len == freq_cap) {
        size_t new_cap = freq_cap ? freq_cap * 2 : 8;
        freq_tab = (struct freq_entry*)realloc(freq_tab,
                                                new_cap * sizeof(struct freq_entry));
        freq_cap = new_cap;
    }
}

static seq_t get_count(seq_t v) {
    for (size_t i = 0; i < freq_len; ++i)
        if (freq_tab[i].val == v)
            return freq_tab[i].count;
    return 0;
}

static void incr_count(seq_t v) {
    for (size_t i = 0; i < freq_len; ++i)
        if (freq_tab[i].val == v) {
            freq_tab[i].count = freq_tab[i].count + 1;
            return;
        }
    ensure_freq_cap();
    freq_tab[freq_len].val   = v;
    freq_tab[freq_len].count = 1;
    ++freq_len;
}

/* --------------------------------- public function --------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* undefined, but harmless */

    /* first call: initialise base cases */
    if (seq_used == 0) {
        ensure_seq_cap(2);
        seq_cache[1] = 1;
        seq_cache[2] = 1;
        seq_used = 2;
        /* frequency for value 1 is 2 */
        incr_count(1);
        incr_count(1);
    }

    if (n <= seq_used)
        return seq_cache[n];

    /* compute missing terms up to n */
    for (idx_t k = seq_used + 1; k <= n; ++k) {
        seq_t val = 1;                           /* default for k=1,2 already set */
        if (k > 2) {
            seq_t v1 = seq_cache[k - 1];
            seq_t v2 = seq_cache[k - 2];
            seq_t f1 = get_count(v1);
            seq_t f2 = get_count(v2);
            val = f1 + f2;
        }
        ensure_seq_cap(k);
        seq_cache[k] = val;
        incr_count(val);
    }
    seq_used = n;
    return seq_cache[n];
}
