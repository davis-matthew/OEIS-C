/*
 * A257470  a(n+1) = least number not occurring earlier such that max{a(n),
 *          a(n+1)} >= 2 min{a(n), a(n+1)}; a(0) = 0.
 * https://oeis.org/A257470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n+1) is the smallest unused integer m such that
 * max{a(n), m} >= 2*min{a(n), m}. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static state maintained across calls */
static seq_t *seq_arr = NULL;   /* array of generated terms   */
static size_t seq_len = 0;      /* number of terms generated  */
static size_t seq_cap = 0;      /* capacity of seq_arr        */

static bool *used = NULL;       /* used indicator array       */
static size_t used_cap = 0;     /* capacity of used array     */

/* ensure used array can hold index 'need'             */
#define ENSURE_USED_CAP(need) do {                                    \
    size_t need_val = (size_t)(need);                                 \
    if (used_cap <= need_val) {                                       \
        size_t new_cap = used_cap ? used_cap * 2 : 256;                \
        while (new_cap <= need_val) new_cap *= 2;                     \
        bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));    \
        if (!tmp) exit(EXIT_FAILURE);                                 \
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool)); \
        used = tmp;                                                   \
        used_cap = new_cap;                                           \
    }                                                                  \
} while (0)

/* ensure seq array can hold the next term */
#define ENSURE_SEQ_CAP() do {                                          \
    if (seq_len >= seq_cap) {                                          \
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;                   \
        seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t)); \
        if (!tmp) exit(EXIT_FAILURE);                                 \
        seq_arr = tmp;                                                \
        seq_cap = new_cap;                                            \
    }                                                                  \
} while (0)

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0] = (seq_t)0;
        seq_len = 1;

        used_cap = 256;
        used = (bool*)calloc(used_cap, sizeof(bool));
        used[0] = true;
    }

    /* generate sequence up to index n */
    while (seq_len <= (size_t)n) {
        seq_t prev = seq_arr[seq_len - 1];
        seq_t half = prev / (seq_t)2;

        /* find smallest unused integer satisfying the condition */
        idx_t cand = 0;
        for (idx_t i = 0; ; ++i) {
            ENSURE_USED_CAP(i);
            if (!used[(size_t)i]) {
                if ((seq_t)i <= half || (seq_t)i >= (seq_t)prev * (seq_t)2) {
                    cand = i;
                    break;
                }
            }
        }

        /* store the new term */
        ENSURE_SEQ_CAP();
        seq_arr[seq_len] = (seq_t)cand;
        ++seq_len;

        /* mark the value as used */
        ENSURE_USED_CAP(cand);
        used[(size_t)cand] = true;
    }

    return seq_arr[(size_t)n];
}
