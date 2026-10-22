/*
 * A370981  Triangle T(n, k), n >= 0, k = 0..n, read and filled in the greedy
 *          way by rows with distinct nonnegative integers, such that for any
 *          n > 0 and any k in 0..n, T(n, k) is congruent modulo n to T(n-1,
 *          k-1) (provided that k > 0) or to T(n-1, k) (provided that k < n).
 * https://oeis.org/A370981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 212, u16 n <= 8889, u32 n <= 8889, u64 n <= 8889,
 *            u128 n <= 8889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a greedy construction of a triangular
 * array T(n,k) with distinct non‑negative integers.
 * For n>0 and 0≤k≤n, T(n,k) is the smallest unused integer that satisfies
 *      T(n,k) ≡ T(n−1,k−1) (mod n)   if k>0
 *      T(n,k) ≡ T(n−1,k)   (mod n)   if k<n
 * The flattened sequence a(n) is the row‑major enumeration of T.
 *
 * Implementation details:
 *   * The triangle is stored in a one‑dimensional array “tri”.
 *   * A bitset “used_bits” marks which integers have already been used.
 *   * All indices and arithmetic are performed with idx_t (uint64_t).
 *   * For the bitset, values are cast to uint64_t; this is safe for the
 *     range of n requested by the problem (n≤10000 → values < 10⁸).
 *   * C and C++ code are both accepted: casts to malloc/realloc
 *     are explicitly written.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Static storage for memoised triangle and used–bitset ---------- */

static seq_t *tri = NULL;          /* linear storage of T(n,k) values       */
static size_t tri_len = 0;          /* number of values stored              */
static size_t tri_cap = 0;          /* current capacity of tri              */

static uint64_t *used_bits = NULL; /* bitset of used integers              */
static size_t used_bits_len = 0;    /* words of used_bits (each word 64 bits)*/

static idx_t rows_completed = 0;    /* number of rows fully generated      */

/* ---------- Helper functions for the bitset ---------- */

/* Mark integer x as used (expanses bitset if necessary) */
static inline void set_used(seq_t x)
{
    uint64_t ux = (uint64_t)(x);
    size_t idx   = (size_t)(ux >> 6);           /* word index   */
    size_t offs  = (size_t)(ux & 63ULL);        /* bit offset   */

    if (idx >= used_bits_len) {
        size_t new_len = idx + 1;
        uint64_t *new_buf = (uint64_t *)realloc(used_bits,
                                                new_len * sizeof(uint64_t));
        if (!new_buf) return;   /* allocation failure → leave slow path */
        for (size_t i = used_bits_len; i < new_len; ++i) new_buf[i] = 0;
        used_bits = new_buf;
        used_bits_len = new_len;
    }
    used_bits[idx] |= (1ULL << offs);
}

/* Query whether integer x has already been used */
static inline bool is_used(seq_t x)
{
    uint64_t ux = (uint64_t)(x);
    size_t idx   = (size_t)(ux >> 6);
    size_t offs  = (size_t)(ux & 63ULL);

    if (idx >= used_bits_len) return false;
    return (used_bits[idx] >> offs) & 1ULL;
}

/* ---------- Candidate search: smallest unused integer ≡ residue (mod mod) ---------- */

static seq_t find_candidate_mod(seq_t residue, seq_t mod)
{
    seq_t cand = residue;
    for (;;) {
        if (!is_used(cand)) return cand;
        cand += mod;
    }
}

/* ---------- Public function a(idx_t n) ---------- */

seq_t a(idx_t n)
{
    /* Initialisation: row 0 contains only 0 */
    if (tri_len == 0) {
        tri_cap = 16;
        tri = (seq_t *)realloc(NULL, tri_cap * sizeof(seq_t));
        tri[tri_len++] = 0;
        set_used(0);
        rows_completed = 1;           /* row 0 is finished                    */
    }

    /* Grow the triangle until the flattened index n is available */
    while (tri_len <= (size_t)n) {
        idx_t m = rows_completed;     /* current row index to generate       */
        if (m == 0) {                 /* already handled by init             */
            ++rows_completed;
            continue;
        }

        /* Index of the first element of the previous row */
        idx_t prev_row_start = (m * (m - 1)) / 2U;

        /* Generate all k from 0 to m in this row */
        for (idx_t k = 0; k <= m; ++k) {
            bool has_left  = (k > 0);
            bool has_right = (k < m);

            seq_t cand_left  = 0;
            seq_t cand_right = 0;

            if (has_left) {
                seq_t parent   = tri[prev_row_start + (k - 1)];
                seq_t residue  = parent % (seq_t)m;
                cand_left      = find_candidate_mod(residue, (seq_t)m);
            }
            if (has_right) {
                seq_t parent   = tri[prev_row_start + k];
                seq_t residue  = parent % (seq_t)m;
                cand_right     = find_candidate_mod(residue, (seq_t)m);
            }

            seq_t chosen;
            if (has_left && (!has_right || cand_left <= cand_right))
                chosen = cand_left;
            else
                chosen = cand_right;

            /* Ensure capacity for tri */
            if (tri_len == tri_cap) {
                tri_cap = tri_cap ? tri_cap * 2 : 16;
                seq_t *new_tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
                if (!new_tri) return 0;   /* allocation failure → abort        */
                tri = new_tri;
            }

            tri[tri_len++] = chosen;
            set_used(chosen);
        }
        ++rows_completed;
    }

    return tri[(size_t)n];
}
