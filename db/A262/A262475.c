/*
 * A262475  Number of (5+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 5, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A262475 – 5+1 × (n+1) 0..1 arrays with row %3==0 and column %5==0
   The sequence is computed by dynamic programming on column remainders.
   For each of the 5 rows we keep the remainder modulo 3 of the binary
   number read from top to bottom; a state is the tuple of five remainders.
   Adding a column whose bits form a 5‑bit number “c” transforms a state
   (r₀,…,r₄) to (r₀′,…,r₄′) with rᵢ′ = (2rᵢ + bitᵢ(c)) % 3.
   Only columns whose numeric value is divisible by 5 may be used –
   there are 13 such columns (0,5,10,…,60).  The DP starts with one
   state (0,0,0,0,0) before any columns.  After k columns the number of
   admissible arrays of width k is the count of paths that reach the
   all‑zero state after k steps.  The required answer a(n) equals this
   count after k = n+1 columns. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Allowed 5‑bit columns (value mod 5 == 0) */
static const uint8_t allowed_cols[13] = {
    0, 5,10,15,20,25,30,35,40,45,50,55,60
};

/* Powers of 3 for encoding states */
static const unsigned pow3[6] = {243,81,27,9,3,1};

/* For each column and each state the resulting state */
static unsigned short transition_tbl[13][729];

/* State buffers */
static seq_t state_cur[729];
static seq_t state_next[729];
static seq_t *state_cur_ptr = state_cur;
static seq_t *state_next_ptr = state_next;

/* Memoised answers – one per number of columns */
#ifdef __cplusplus
static std::vector<seq_t> answers;
#else
static seq_t *answers = NULL;
static size_t answers_sz = 0;
#endif
static idx_t max_step = 0;   /* highest computed number of columns */

/* Initialisation flag */
static bool initialized = false;

/* Compute transition table – called once */
static void init(void)
{
    /* initialise state buffers */
    for (int i = 0; i < 729; ++i) {
        state_cur_ptr[i] = 0;
        state_next_ptr[i] = 0;
    }
    state_cur_ptr[0] = 1;   /* empty array → state (0,0,0,0,0) */

    /* build transition table */
    for (int k = 0; k < 13; ++k) {
        uint8_t col = allowed_cols[k];
        for (int s = 0; s < 729; ++s) {
            int new_state = 0;
            for (int i = 0; i < 6; ++i) {
                int old_r = (s / pow3[i]) % 3;
                int bit   = (col >> (5 - i)) & 1;
                int new_r = (old_r * 2 + bit) % 3;
                new_state += new_r * pow3[i];
            }
            transition_tbl[k][s] = (unsigned short) new_state;
        }
    }

    /* memory for answers */
#ifdef __cplusplus
    answers.resize(2);
#else
    answers = (seq_t*)calloc(2, sizeof(seq_t));
    answers_sz = 2;
#endif

    max_step = 0;
    initialized = true;
}

/* Ensure answers array is long enough */
static void ensure_answers_size(idx_t step)
{
    if (step + 1 <= max_step) return;

#ifdef __cplusplus
    if (step + 1 > answers.size())
        answers.resize(step + 1);   /* vector guarantees init */
#else
    if (step + 1 > answers_sz) {
        size_t new_sz = step + 1;
        seq_t *tmp = (seq_t*)realloc(answers, new_sz * sizeof(seq_t));
        if (!tmp) {
            /* out‑of‑memory – aborting is safest */
            exit(EXIT_FAILURE);
        }
        /* zero new elements */
        for (size_t i = answers_sz; i < new_sz; ++i)
            tmp[i] = 0;
        answers = tmp;
        answers_sz = new_sz;
    }
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* number of columns to compute */
    idx_t target = n + 1;

    /* extend answer array if necessary */
    if (target + 1 > max_step) {
        ensure_answers_size(target);
    }

    /* perform DP steps up to target */
    for (idx_t step = max_step + 1; step <= target; ++step) {
        /* reset next buffer */
        for (int i = 0; i < 729; ++i)
            state_next_ptr[i] = 0;

        /* propagate through all allowed columns */
        for (int k = 0; k < 13; ++k) {
            unsigned short *trans = transition_tbl[k];
            for (int s = 0; s < 729; ++s) {
                seq_t val = state_cur_ptr[s];
                if (!val) continue;
                state_next_ptr[ trans[s] ] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = state_cur_ptr;
        state_cur_ptr = state_next_ptr;
        state_next_ptr = tmp;

        /* record answer for this step (number of columns) */
#ifdef __cplusplus
        answers[step] = state_cur_ptr[0];
#else
        answers[step] = state_cur_ptr[0];
#endif
    }
    max_step = target;

    /* return a(n) which corresponds to target columns */
#ifdef __cplusplus
    return answers[target];
#else
    return answers[target];
#endif
}
