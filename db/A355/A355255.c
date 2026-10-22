/*
 * A355255  Irregular table read by rows: a(n,k) gives the number of distinct
 *          necklaces that appear in the following procedure: starting with
 *          the n-bead, (0,1)-necklace given by k written in binary,
 *          repeatedly take the first differences (mod 2) of the beads. 0 <= k
 *          < 2^n.
 * https://oeis.org/A355255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, look
 * Formula:   a(n) is the (n‑th) term of the irregular table a(r,k) read row
 *            by row: a(n) = a(r,k) where r = floor(log2(n+1)) and k = n –
 *            (2^r – 1). a(r,k) is the number of distinct necklaces that
 *            appear when iterating the first‑difference mod 2 process
 *            starting from the binary word of length r represented by k
 * Range:     u8 n <= 8190, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for fixed integer types, memory allocation and
   bit operations. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Maximum row that covers all indices used in the test suite.
   2^16 gives 65535 total entries, more than enough for the limits
   given (indices up to ~10000). */
#define MAX_ROW 16

/* Forward declaration of the function required by the problem. */
seq_t a(idx_t n);

/*------------------------------------------------------------------*/
/*   Private data used to build and cache the table a(r,k)          */
/*------------------------------------------------------------------*/
static seq_t *a_table[MAX_ROW];     /* pointers to rows of a(r,k)   */
static size_t row_size[MAX_ROW];    /* number of k in each row      */
static int  initialized = 0;        /* flag to indicate init done   */

/*------------------------------------------------------------------*/
/*   Utility: floor(log2(x)) for unsigned 64‑bit integer, x>0       */
/*------------------------------------------------------------------*/
static inline int log2_floor_uint64(uint64_t x)
{
    int r = 0;
    /* shift right until x == 1; counts number of bits – 1 */
    x >>= 1;
    while (x) {
        r++;
        x >>= 1;
    }
    return r;
}

/*------------------------------------------------------------------*/
/*   Build all rows of the table a(r,k).  Only done once.            */
/*------------------------------------------------------------------*/
static void build_table(void)
{
    if (initialized) return;
    initialized = 1;

    for (int r = 0; r < MAX_ROW; r++) {
        size_t sz = 1u << r;                /* 2^r entries in row r */
        row_size[r] = sz;
        a_table[r] = (seq_t *)malloc(sz * sizeof(seq_t));
        if (a_table[r] == NULL) abort();   /* fatal on allocation failure */

        if (r == 0) {                     /* special case: empty word */
            a_table[0][0] = (seq_t)1;      /* only the zero necklace */
            continue;
        }

        uint64_t mask = (1ULL << r) - 1;    /* keep only r bits */

        /* Pre‑compute canonical representatives and next pattern. */
        uint16_t *canonical = (uint16_t *)malloc(sz * sizeof(uint16_t));
        uint16_t *next_pat   = (uint16_t *)malloc(sz * sizeof(uint16_t));
        if (!canonical || !next_pat) abort();

        for (size_t p = 0; p < sz; ++p) {
            /* canonical: smallest rotation of p */
            uint16_t cur = (uint16_t)p;
            uint16_t best = cur;
            for (int step = 1; step < r; ++step) {
                cur = (uint16_t)(((cur << 1) | (cur >> (r - 1))) & mask);
                if (cur < best) best = cur;
            }
            canonical[p] = best;

            /* next pattern under the difference map */
            uint16_t rot = (uint16_t)(((p << 1) | (p >> (r - 1))) & mask);
            next_pat[p] = (uint16_t)(p ^ rot);
        }

        /* Buffers reused for each k: visited states and seen necklaces */
        unsigned char *visited = (unsigned char *)malloc(sz);
        unsigned char *seen_neck = (unsigned char *)malloc(sz);
        if (!visited || !seen_neck) abort();

        for (size_t k = 0; k < sz; ++k) {
            memset(visited, 0, sz);
            memset(seen_neck, 0, sz);

            size_t cur = k;
            size_t distinct = 0;
            while (!visited[cur]) {
                visited[cur] = 1;
                uint16_t canon = canonical[cur];
                if (!seen_neck[canon]) {
                    seen_neck[canon] = 1;
                    ++distinct;
                }
                cur = next_pat[cur];
            }
            a_table[r][k] = (seq_t)distinct;
        }

        free(visited);
        free(seen_neck);
        free(canonical);
        free(next_pat);
    }
}

/*------------------------------------------------------------------*/
/*   Public function required by the problem description            */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    build_table();

    /* Determine the row r from the index n */
    int r = log2_floor_uint64((uint64_t)n + 1u);
    size_t row_start = (1u << r) - 1u;     /* cumulative count before row r */
    size_t k = n - row_start;              /* column within row r */

    return a_table[r][k];
}
