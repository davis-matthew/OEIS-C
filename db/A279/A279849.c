/*
 * A279849  Rows of the 48 self-orthogonal Latin squares of order 4,
 *          lexicographically sorted.
 * https://oeis.org/A279849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   not stated
 * Range:     u8 n <= 768, u16 n <= 768, u32 n <= 768, u64 n <= 768,
 *            u128 n <= 768, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279849: rows of the 48 self‑orthogonal Latin squares of order 4,
   lexicographically sorted.  The sequence has 48 × 4 × 4 = 768 terms. */
/* The code builds all 48 squares once and returns a(n) modulo the
   sequence length, as per the OEIS offset (a(1) is the first element). */

#include <stdlib.h>
#include <stdbool.h>

/* Data structure to store one 4 × 4 Latin square */
typedef struct {
    seq_t data[16];          /* flattened 4×4 array */
} Square;

/* Global variables used during generation */
static int L[4][4];
static bool row_used[4][5];
static bool col_used[4][5];

static Square squares[48];
static size_t square_count;

/* Recursive backtracking to build all Latin squares */
static void backtrack(int pos)
{
    if (pos == 16) {
        /* Check the self‑orthogonality condition */
        bool used[5][5] = {{false}};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int a = L[i][j];
                int b = L[j][i];
                if (used[a][b]) return;      /* duplicate pair */
                used[a][b] = true;
            }
        }
        /* Store the square */
        if (square_count < 48) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    squares[square_count].data[i * 4 + j] = (seq_t)L[i][j];
            ++square_count;
        }
        return;
    }

    int i = pos / 4;
    int j = pos % 4;

    for (int val = 1; val <= 4; ++val) {
        if (!row_used[i][val] && !col_used[j][val]) {
            row_used[i][val] = col_used[j][val] = true;
            L[i][j] = val;
            backtrack(pos + 1);
            row_used[i][val] = col_used[j][val] = false;
        }
    }
}

/* Comparator for sorting squares lexicographically */
static int cmp_squares(const void *p1, const void *p2)
{
    const Square *s1 = (const Square *)p1;
    const Square *s2 = (const Square *)p2;
    for (int k = 0; k < 16; ++k) {
        if (s1->data[k] < s2->data[k]) return -1;
        if (s1->data[k] > s2->data[k]) return  1;
    }
    return 0;
}

/* Build the flattened sequence from the sorted squares */
static void build_sequence(seq_t *seq)
{
    /* initialise usage tables */
    for (int i = 0; i < 4; ++i)
        for (int v = 1; v <= 4; ++v) {
            row_used[i][v] = false;
            col_used[i][v] = false;
        }
    square_count = 0;

    backtrack(0);

    /* sort the 48 squares */
    qsort(squares, square_count, sizeof(Square), cmp_squares);

    /* flatten into seq */
    size_t idx = 0;
    for (size_t s = 0; s < square_count; ++s)
        for (int k = 0; k < 16; ++k)
            seq[idx++] = squares[s].data[k];
}

/* Public function: returns a(n) */
seq_t a(idx_t n)
{
    /* Number of terms in the sequence */
    const idx_t TERMS = 48 * 4 * 4;    /* 768 */

    static seq_t *seq = NULL;
    static bool ready = false;

    if (!ready) {
        /* Allocate static array */
        seq = (seq_t *)malloc(TERMS * sizeof(seq_t));
        if (seq == NULL) {              /* allocation failure – abort */
            for (;;) ;                   /* safe‑guard for compilation */
        }
        build_sequence(seq);
        ready = true;
    }

    /* a(1) is the first term; use modulo for safety */
    idx_t k = (n - 1) % TERMS;
    return seq[(size_t)k];
}
