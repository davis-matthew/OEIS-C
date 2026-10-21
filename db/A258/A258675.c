/*
 * A258675  Number of (n+2) X (2+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum equal to the antidiagonal sum.
 * https://oeis.org/A258675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:
        a(1) = 1600
        a(n) = 9*a(n-1) - 36*a(n-2) + 180*a(n-3) - 2880*a(n-5) + 9216*a(n-6)
             - 36864*a(n-7) + 65536*a(n-8)   (n ≥ 9)
    Implementation: dynamic programming over 4‑column binary rows, not the
    recurrence directly.  For each pair of consecutive rows we pre‑compute
    the set of admissible next rows, then count paths of length n+2-2 = n+? .
    Memoise results for all n needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- Helper: bit extraction ------------------------------------------ */
static inline int bit_at(int row, int pos) { return (row >> pos) & 1; }

/* --- Compatibility test for (a,b,c) rows --------------------------- */
static int compatible(int a, int b, int c)
{
    /* window 1: columns 0,1,2 */
    int d1 = bit_at(a,0)+bit_at(b,1)+bit_at(c,2);
    int a1 = bit_at(a,2)+bit_at(b,1)+bit_at(c,0);
    if (d1 == a1) return 0;
    /* window 2: columns 1,2,3 */
    int d2 = bit_at(a,1)+bit_at(b,2)+bit_at(c,3);
    int a2 = bit_at(a,3)+bit_at(b,2)+bit_at(c,1);
    return d2 != a2;
}

/* --- Main function requested ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1 */

    /* Static data structures ---------------------------------------- */
    static int initialized = 0;

    /* adjacency lists: nextStateList[s*16+idx] gives next state indices */
    static unsigned short *nextStateList = NULL;   /* size 256*16 */
    static unsigned char  *nextStateCount = NULL;   /* size 256 */

    /* DP state vectors (size 256) */
    static seq_t *cur = NULL;
    static seq_t *next_buf = NULL;

    /* cache for a(n) values */
    static seq_t *a_cache = NULL;
    static size_t  a_cache_cap = 0;   /* allocated capacity */
    static size_t  a_cache_len = 0;   /* number of stored terms */
    static size_t  max_n_done = 0;    /* how many terms are computed */

    /* ----------------------------------------------------------------- */
    /* First time: create adjacency lists and DP vectors                */
    if (!initialized) {
        /* allocate adjacency storage */
        nextStateList = (unsigned short *)malloc(256*16 * sizeof(unsigned short));
        nextStateCount = (unsigned char  *)malloc(256 * sizeof(unsigned char));
        if (!nextStateList || !nextStateCount) return 0; /* allocation fail */

        /* compute for each state (a,b) the admissible next rows c */
        for (size_t s = 0; s < 256; ++s) {
            int a = (int)(s >> 4);
            int b = (int)(s & 0xF);
            unsigned char cnt = 0;
            for (int c = 0; c < 16; ++c) {
                if (compatible(a,b,c)) {
                    unsigned short t = ((unsigned short)b << 4) | (unsigned short)c;
                    nextStateList[s*16 + cnt++] = t;
                }
            }
            nextStateCount[s] = cnt;
        }

        /* allocate DP vectors */
        cur = (seq_t *)malloc(256 * sizeof(seq_t));
        next_buf = (seq_t *)malloc(256 * sizeof(seq_t));
        if (!cur || !next_buf) return 0;

        /* initial distribution for two rows: all 256 states are possible */
        for (size_t i = 0; i < 256; ++i) cur[i] = (seq_t)1;
        /* no terms computed yet */
        max_n_done = 0;
        a_cache_cap = 0;
        a_cache_len = 0;

        initialized = 1;
    }

    /* ----------------------------------------------------------------- */
    /* Extend computations up to required n (if not already done)        */
    size_t req = (size_t)n;
    while (max_n_done < req) {
        /* next_buf := 0 */
        for (size_t t = 0; t < 256; ++t) next_buf[t] = (seq_t)0;

        /* transition */
        for (size_t s = 0; s < 256; ++s) {
            seq_t cnt_s = cur[s];
            if (cnt_s == (seq_t)0) continue;
            unsigned char m = nextStateCount[s];
            for (unsigned char k = 0; k < m; ++k) {
                unsigned short t = nextStateList[s*16 + k];
                next_buf[t] += cnt_s;
            }
        }

        /* sum current counts = a(max_n_done + 1) */
        seq_t total = (seq_t)0;
        for (size_t t = 0; t < 256; ++t) total += next_buf[t];

        /* ensure cache capacity */
        if (max_n_done >= a_cache_cap) {
            size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
            while (new_cap <= max_n_done) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            a_cache = tmp;
            a_cache_cap = new_cap;
            a_cache_len = new_cap;
        }

        /* store */
        a_cache[max_n_done] = total;
        ++max_n_done;

        /* swap cur and next_buf */
        seq_t *tmp = cur;  cur = next_buf;  next_buf = tmp;
    }

    /* ----------------------------------------------------------------- */
    return a_cache[n-1];      /* a(1) is stored at index 0 */
}
