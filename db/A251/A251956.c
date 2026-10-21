/*
 * A251956  Number of (n+2)X(1+2) 0..4 arrays with every row, column, diagonal
 *          or antidiagonal in each 3X3 subblock summing to a prime.
 * https://oeis.org/A251956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 arrays over {0..4} with every row,
 *            column, diagonal or anti-diagonal in each 3×3 subblock summing
 *            to a prime. Computed by dynamic programming over successive rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Size of state space: 5³ rows × 5³ rows = 15625 */
enum { S = 125 * 125 };

/* Prime indicator for sums 0..12. */
static const bool prime_sum[13] = {
    /*  0  1  2  3  4  5  6  7  8  9 10 11 12 */
    false, false, true, true, false, true, false, true,
    false, false, false, true, false
};

/* Table of the 125 possible rows, each as 3 digits in base‑5. */
static unsigned char row_digits[125][3];

/* Transition structure: from a pair (prev1,prev2) to compatible next rows. */
typedef struct {
    unsigned short next[125]; /* indices of compatible rows */
    unsigned short len;       /* number of entries in next[] */
} trans_t;

static trans_t transitions[S];
static bool transitions_initialized = false;

/* Build the transition table once. */
static void init_transition(void) {
    if (transitions_initialized) return;

    /* Convert each row index to its 3‑digit representation. */
    for (int r = 0; r < 125; ++r) {
        row_digits[r][0] = (r / 25) % 5;    /* most significant digit */
        row_digits[r][1] = (r / 5) % 5;     /* middle digit */
        row_digits[r][2] = r % 5;           /* least significant digit */
    }

    /* Pre‑compute allowed triples of consecutive rows. */
    for (int i = 0; i < 125; ++i) {
        for (int j = 0; j < 125; ++j) {
            trans_t *t = &transitions[i * 125 + j];
            t->len = 0;
            for (int k = 0; k < 125; ++k) {
                unsigned char *a = row_digits[i];
                unsigned char *b = row_digits[j];
                unsigned char *c = row_digits[k];
                int rs0 = a[0] + a[1] + a[2];
                int rs1 = b[0] + b[1] + b[2];
                int rs2 = c[0] + c[1] + c[2];
                int cs0 = a[0] + b[0] + c[0];
                int cs1 = a[1] + b[1] + c[1];
                int cs2 = a[2] + b[2] + c[2];
                int d1  = a[0] + b[1] + c[2];
                int d2  = a[2] + b[1] + c[0];
                if (!prime_sum[rs0] || !prime_sum[rs1] || !prime_sum[rs2] ||
                    !prime_sum[cs0] || !prime_sum[cs1] || !prime_sum[cs2] ||
                   !prime_sum[d1]  || !prime_sum[d2])
                    continue;
                t->next[t->len++] = (unsigned short)k;
            }
        }
    }

    transitions_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_transition();

    idx_t rows = n + 2;   /* number of rows in the array */

    /* Two DP arrays: dp_cur holds counts for current pair of rows,
       dp_next accumulates counts for the next pair. */
    static seq_t dp_cur[S];
    static seq_t dp_next[S];

    /* Initialise with all pairs of first two rows allowed. */
    for (size_t i = 0; i < S; ++i) dp_cur[i] = (seq_t)1;

    /* Process each additional row starting with the third. */
    for (idx_t pos = 2; pos < rows; ++pos) {
        /* Zero next layer. */
        for (size_t i = 0; i < S; ++i) dp_next[i] = (seq_t)0;

        /* Transition from each current state to compatible next rows. */
        for (int i = 0; i < 125; ++i) {
            for (int j = 0; j < 125; ++j) {
                size_t idx = i * 125 + j;
                seq_t val = dp_cur[idx];
                if (!val) continue;
                const trans_t *t = &transitions[idx];
                for (unsigned short u = 0; u < t->len; ++u) {
                    unsigned short k = t->next[u];
                    size_t next_idx = j * 125 + k;
                    dp_next[next_idx] += val;
                }
            }
        }

        /* Prepare for next iteration. */
        for (size_t i = 0; i < S; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all final counts. */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < S; ++i) ans += dp_cur[i];
    return ans;
}
