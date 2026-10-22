/*
 * A326764  Lexicographically earliest array T(x,y,z) of nonnegative integers
 *          with x, y, z >= 0, such that the terms alongside any line parallel
 *          to any of the 13 axes of rotation of a cube are distinct.
 * https://oeis.org/A326764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9413, u16 n <= 9413, u32 n <= 9413, u64 n <= 9413,
 *            u128 n <= 9413, big any n
 * Helpers:   get_invariants
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Implementation of A326764:  Lexicographically earliest array T(x,y,z)
   of nonnegative integers such that the terms along any line parallel
   to any of the 13 axes of rotation of a cube are distinct.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_invariants.h"

/* number of directional axes of a cube */
#define DIRS 13
/* size of the hash tables (power of two) */
#define HT_SIZE (1 << 17)

/*--- line metadata ---------------------------------------------------*/

typedef struct LineInfo {
    int32_t key1, key2;      /* invariant keys */
    size_t bits_size;        /* number of 64‑bit words used */
    uint64_t *bits;          /* bitset of values that already appear */
} LineInfo;

/*--- hash table for lines --------------------------------------------*/

typedef struct {
    size_t size, mask;
    LineInfo **buckets;
} DirHT;

static DirHT dirs[DIRS];
static bool dirs_inited = false;

/*--- global sequence storage ---------------------------------------*/

static seq_t *seq   = NULL;     /* array of values */
static idx_t  seq_len   = 0;    /* number of stored terms */
static idx_t  seq_cap   = 0;    /* allocated capacity of seq */

/*--- state for generating the next coordinate ---------------------*/

static int32_t cur_s = 0, cur_t = 0, cur_x = 0;  /* next point's (s,t,x) */

/*--- helpers -------------------------------------------------------*/

static void init_dir(DirHT *dh)
{
    dh->size = HT_SIZE;
    dh->mask = HT_SIZE - 1;
    dh->buckets = (LineInfo **)calloc(dh->size, sizeof(LineInfo *));
}

static void init_dirs(void)
{
    if (dirs_inited) return;
    for (int i = 0; i < DIRS; ++i) init_dir(&dirs[i]);
    dirs_inited = true;
}

static uint64_t make_key(int32_t k1, int32_t k2)
{
    return (((uint64_t)(uint32_t)k1) << 32) | (uint32_t)k2;
}

static LineInfo *line_fetch(DirHT *dh, int32_t k1, int32_t k2)
{
    uint64_t key = make_key(k1, k2);
    size_t idx = key & dh->mask;
    while (1) {
        LineInfo *li = dh->buckets[idx];
        if (!li) {                          /* create new line */
            li = (LineInfo *)malloc(sizeof(LineInfo));
            li->key1 = k1; li->key2 = k2;
            li->bits_size = 0;
            li->bits = NULL;
            dh->buckets[idx] = li;
            return li;
        }
        if (li->key1 == k1 && li->key2 == k2) return li;
        idx = (idx + 1) & dh->mask;         /* linear probe */
    }
}

static inline bool bit_set(const LineInfo *li, seq_t val)
{
    size_t w = (size_t)val >> 6, b = (size_t)val & 63;
    return (w < li->bits_size) && ((li->bits[w] >> b) & 1ULL);
}

static inline void bit_set_line(LineInfo *li, seq_t val)
{
    size_t w = (size_t)val >> 6, b = (size_t)val & 63;
    if (w >= li->bits_size) {
        size_t newsz = w + 1;
        if (newsz < li->bits_size * 2) newsz = li->bits_size * 2;
        if (newsz == 0) newsz = 1;
        uint64_t *tmp = (uint64_t *)realloc(li->bits, newsz * sizeof(uint64_t));
        for (size_t i = li->bits_size; i < newsz; ++i) tmp[i] = 0;
        li->bits = tmp;
        li->bits_size = newsz;
    }
    li->bits[w] |= (1ULL << b);
}

/* invariants for each of the 13 axes --------------------------------*/

/* advance to next coordinate in the lexicographic order */
static void incr_coord(void)
{
    if (cur_x < cur_t) {
        cur_x++;
    } else { /* cur_x == cur_t */
        if (cur_t < cur_s) {
            cur_t++;
            cur_x = 0;
        } else {        /* cur_t == cur_s */
            cur_s++;
            cur_t = 0;
            cur_x = 0;
        }
    }
}

/* grow sequence array if needed */
static void grow_seq(idx_t need)
{
    if (need < seq_cap) return;
    idx_t newcap = seq_cap ? seq_cap << 1 : 64;
    while (newcap <= need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq = tmp;
    seq_cap = newcap;
}

/* extend the generated array up to index n (inclusive) */
static void extend_to(idx_t n)
{
    init_dirs();                /* ensure hash tables ready */

    grow_seq(n);
    for (idx_t idx = seq_len; idx <= n; ++idx) {
        /* coordinates of current point */
        int32_t x = cur_x;
        int32_t y = cur_t - cur_x;
        int32_t z = cur_s - cur_t;

        /* compute invariants for all dirs and fetch lines */
        int32_t k1[DIRS], k2[DIRS];
        LineInfo *lines[DIRS];
        for (int d = 0; d < DIRS; ++d) {
            get_invariants(d, x, y, z, &k1[d], &k2[d]);
            lines[d] = line_fetch(&dirs[d], k1[d], k2[d]);
        }

        /* find smallest non‑negative integer not yet used on any line */
        seq_t val = 0;
        for (;;) {
            bool ok = true;
            for (int d = 0; d < DIRS; ++d) {
                if (bit_set(lines[d], val)) { ok = false; break;}
            }
            if (ok) break;
            ++val;
        }

        /* store value and mark it on all lines */
        seq[idx] = val;
        for (int d = 0; d < DIRS; ++d) bit_set_line(lines[d], val);

        /* advance to next coordinate */
        incr_coord();
    }
    seq_len = n + 1;
}

/*--- public API ------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n >= seq_len) extend_to(n);
    return seq[n];
}
