/*
 * A326854  BII-numbers of T_0 (costrict), pairwise intersecting set-systems
 *          where every two vertices appear together in some edge
 *          (cointersecting).
 * https://oeis.org/A326854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate integers and test each one for the BII-number
   condition defined in A326854 (pairwise intersecting, cointersecting,
   and T0 set-systems).  The n-th valid integer (1-indexed) is returned. */

#include <stddef.h>   /* for NULL, size_t */
#include <stdint.h>   /* for uint64_t */
#include <stdbool.h>  /* for bool */

/* Test whether an integer val is a BII-number. */
static bool is_valid(idx_t val)
{
    // Edge and vertex data structures, limited to 64 edges/vertices.
    uint64_t vertex_mask = 0;            /* bit j set if vertex j appears */
    uint64_t inc_mask[65] = {0};         /* inc_mask[v]: bitset of edges containing vertex v */
    uint64_t edge_mask[65] = {0};         /* edge_mask[e]: bitset of vertices in edge e */
    size_t edge_cnt = 0;                 /* number of edges processed */

    idx_t tmp = val;
    idx_t pos = 1;                       /* 1-based bit position (LSB = 1) */
    while (tmp != 0) {
        if (tmp & 1ULL) {                /* pos is a binary index of val */
            idx_t tpos = pos;
            uint8_t p = 1;               /* vertex index (1-based) */
            uint64_t e_mask = 0;        /* vertex mask of the current edge */

            while (tpos != 0) {
                if (tpos & 1ULL) {
                    vertex_mask |= (1ULL << (p - 1));
                    inc_mask[p]  |= (1ULL << edge_cnt);
                    e_mask       |= (1ULL << (p - 1));
                }
                tpos >>= 1;
                p++;
            }
            edge_mask[edge_cnt] = e_mask;
            edge_cnt++;
        }
        tmp >>= 1;
        pos++;
    }

    /* Pairwise intersecting: every pair of edges shares at least one vertex. */
    for (size_t a = 0; a < edge_cnt; ++a) {
        for (size_t b = a + 1; b < edge_cnt; ++b) {
            if ((edge_mask[a] & edge_mask[b]) == 0ULL)
                return false;                /* disjoint edges found */
        }
    }

    /* Cointersecting & T0 checks on vertices. */
    for (int a = 1; a < 65; ++a) {
        if (!(vertex_mask & (1ULL << (a - 1)))) continue;
        uint64_t mask_a = inc_mask[a];
        for (int b = a + 1; b < 65; ++b) {
            if (!(vertex_mask & (1ULL << (b - 1)))) continue;
            uint64_t mask_b = inc_mask[b];

            if (mask_a == mask_b)            /* identical incidence pattern → T0 fails */
                return false;
            if ((mask_a & mask_b) == 0ULL)    /* no common edge → cointersecting fails */
                return false;
        }
    }

    return true;   /* all conditions satisfied */
}

/* Cache of found BII-numbers, sufficient for the required range. */
#define MAX_CACHE_SIZE 20000
static seq_t cache[MAX_CACHE_SIZE];
static idx_t cache_len = 0;
static idx_t next_candidate = 0;

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: sequence is defined for n ≥ 1 */

    while (cache_len < n) {
        if (is_valid(next_candidate)) {
            if (cache_len < MAX_CACHE_SIZE)
                cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
