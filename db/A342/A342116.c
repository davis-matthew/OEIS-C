/*
 * A342116  Lexicographically latest sequence of distinct nonnegative integers
 *          such that the multisets of frequencies of digits in the factorial
 *          base representations of n and of a(n) are the same.
 * https://oeis.org/A342116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A342116.  This function computes the lexicographically latest
    permutation of the nonnegative integers that preserves the
    multiset of frequencies of digits in the factorial‑base
    representation.  For each equivalence class of numbers sharing the
    same multiset, the numbers are sorted ascending and paired
    symmetrically (first ↔ last, second ↔ second‑last, etc.).  The
    resulting array is cached and expanded lazily to cover any
    requested index n.  The implementation is valid in both C11 and
    C++17; it uses only the permitted integer operations on seq_t.  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* External integer types – defined in the build. */
#ifndef idx_t
typedef uint64_t idx_t;
#endif
#ifndef seq_t
/* seq_t is provided by the build; the code below only uses the
   arithmetic + - * / % and comparisons allowed for seq_t. */
#endif

/* ---------- factorial tables ---------- */
static idx_t FACTORIAL[21];
static int  FACTORIAL_INIT = 0;

static void init_factorials(void)
{
    if (FACTORIAL_INIT) return;
    FACTORIAL[0] = 1;
    for (int i = 1; i < 21; ++i)
        FACTORIAL[i] = FACTORIAL[i - 1] * (idx_t)i;
    FACTORIAL_INIT = 1;
}

/* Return the smallest factorial > n.  For n <= 20!, the result is
   guaranteed to exist in FACTORIAL[1 .. 20]. */
static idx_t next_factorial_limit(idx_t n)
{
    init_factorials();
    for (int i = 1; i < 21; ++i)
        if (FACTORIAL[i] > n)
            return FACTORIAL[i];
    return FACTORIAL[20];          /* fallback – never used for our limits */
}

/* ---------- pattern entry ---------- */
struct Entry {
    idx_t n;                    /* the number itself                     */
    uint8_t key_len;            /* length of the key buffer              */
    uint8_t key[25];            /* sorted list of non‑zero freq values   */
};

/* Compute the key for a number n and store it into *e. */
static void compute_entry(idx_t n, struct Entry *e)
{
    unsigned char freq[21] = {0};
    if (n == 0) {
        e->key_len = 0;
        e->n = 0;
        return;
    }

    idx_t temp = n;
    int i = 1;
    while (temp > 0) {
        unsigned char digit = temp % (i + 1);
        freq[digit]++;            /* count occurrences of this digit      */
        temp /= (i + 1);
        ++i;                      /* next factorial position             */
    }

    unsigned char keybuf[21];
    int keylen = 0;
    for (int d = 0; d < 21; ++d)
        if (freq[d] > 0)
            keybuf[keylen++] = freq[d];

    /* sort the key buffer ascending */
    for (int a = 0; a < keylen; ++a)
        for (int b = a + 1; b < keylen; ++b)
            if (keybuf[a] > keybuf[b]) {
                unsigned char t = keybuf[a];
                keybuf[a] = keybuf[b];
                keybuf[b] = t;
            }

    e->n = n;
    e->key_len = (uint8_t)keylen;
    memcpy(e->key, keybuf, keylen);
}

/* Comparator for sorting entries by their key. */
static int cmp_entries(const void *A, const void *B)
{
    const struct Entry *a = (const struct Entry *)A;
    const struct Entry *b = (const struct Entry *)B;
    if (a->key_len != b->key_len)
        return (int)a->key_len - (int)b->key_len;
    return memcmp(a->key, b->key, a->key_len);
}

/* Comparator for sorting numbers (idx_t). */
static int cmp_idx_t(const void *A, const void *B)
{
    const idx_t *a = (const idx_t *)A;
    const idx_t *b = (const idx_t *)B;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* ---------- cache storage ---------- */
static seq_t *CACHE = NULL;          /* array of a(n) values            */
static idx_t  CACHE_SIZE = 0;        /* size of CACHE                   */

/* Build the cache up to the given limit (exclusive).  Such that
   CACHE[0 .. limit-1] are filled with the correct a(n). */
static void build_cache(idx_t limit)
{
    /* free previous cache */
    if (CACHE) {
        free(CACHE);
        CACHE = NULL;
    }
    CACHE_SIZE = 0;

    size_t l = (size_t)limit;
    struct Entry *entries = (struct Entry *)malloc(l * sizeof(struct Entry));
    seq_t *new_cache = (seq_t *)malloc(l * sizeof(seq_t));

    /* initialise all cache entries to zero to properly construct
       each seq_t element (works for integral and arbitrary‑precision
       types). */
    for (size_t i = 0; i < l; ++i)
        new_cache[i] = 0;

    init_factorials();

    for (size_t i = 0; i < l; ++i)
        compute_entry((idx_t)i, &entries[i]);

    /* sort entries by key */
    qsort(entries, l, sizeof(struct Entry), cmp_entries);

    /* process groups of identical keys */
    size_t pos = 0;
    while (pos < l) {
        size_t start = pos;
        /* find end of this key group */
        while (pos < l &&
               entries[pos].key_len == entries[start].key_len &&
               memcmp(entries[pos].key, entries[start].key, entries[start].key_len) == 0)
            ++pos;
        size_t sz = pos - start;

        /* collect numbers in this group */
        idx_t *group = (idx_t *)malloc(sz * sizeof(idx_t));
        for (size_t j = 0; j < sz; ++j)
            group[j] = entries[start + j].n;

        /* sort the numbers ascending */
        qsort(group, sz, sizeof(idx_t), cmp_idx_t);

        /* assign symmetric partners */
        for (size_t j = 0; j < sz; ++j)
            new_cache[(size_t)group[j]] = (seq_t)group[sz - 1 - j];

        free(group);
    }

    free(entries);
    CACHE = new_cache;
    CACHE_SIZE = limit;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n >= CACHE_SIZE) {
        idx_t new_limit = next_factorial_limit(n);
        build_cache(new_limit);
    }
    return CACHE[(size_t)n];
}
