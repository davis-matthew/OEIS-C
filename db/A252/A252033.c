/*
 * A252033  Number of (n+2)X(1+2) 0..4 arrays with every 3X3 subblock row and
 *          column sum nonprime and every diagonal and antidiagonal sum prime.
 * https://oeis.org/A252033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 36, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A252033: Count (n+2)×3 arrays over {0,1,2,3,4} whose every 3×3 subblock
 * has all row‑ and column‑sums non‑prime and both diagonals’ sums prime.
 *
 * The algorithm builds all admissible rows (row sums non‑prime) and
 * precomputes all admissible transitions of three consecutive rows.
 * For a given n we initialize a DP over pairs of rows and extend it
 * step by step n times (adding one more row each step).  The final
 * sum of DP states gives a(n).
 *
 * Complexity:  O(STATE + n·STATE), where STATE = 125² = 15625.
 *           Memory:  O(STATE).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Compile‑time constants */
#define N_ROW 125
#define STATE (N_ROW * N_ROW)

/* Prime table for numbers 0..12 */
static const bool isPrime[13] = {
    /*0,1*/ false, false,
    /*2*/  true,
    /*3*/  true,
    /*4*/  false,
    /*5*/  true,
    /*6*/  false,
    /*7*/  true,
    /*8*/  false,
    /*9*/  false,
    /*10*/ false,
    /*11*/ true,
    /*12*/ false
};

seq_t a(idx_t n)
{
    /* Static workspace for repeated calls */
    static bool initialized = false;
    static bool rowNonPrime[N_ROW];
    static uint8_t rowDigits[N_ROW][3];
    static seq_t dpA[STATE];
    static seq_t dpB[STATE];
    static int *next_start = NULL;  /* prefix sums of transitions */
    static int *next_to    = NULL;  /* flat list of transitions */

    /* Precompute admissible rows and transitions once */
    if (!initialized) {
        /* Classify each row by its digit sum */
        for (int idx = 0; idx < N_ROW; ++idx) {
            int t = idx;
            rowDigits[idx][0] = t % 5; t /= 5;
            rowDigits[idx][1] = t % 5; t /= 5;
            rowDigits[idx][2] = t % 5;
            int sum = rowDigits[idx][0] + rowDigits[idx][1] + rowDigits[idx][2];
            rowNonPrime[idx] = !isPrime[sum];
        }

        /* Count transitions per state */
        int *trans_counts = (int*)calloc(STATE, sizeof(int));
        for (int a = 0; a < N_ROW; ++a) {
            if (!rowNonPrime[a]) continue;
            for (int b = 0; b < N_ROW; ++b) {
                if (!rowNonPrime[b]) continue;
                int state = a * N_ROW + b;
                for (int c = 0; c < N_ROW; ++c) {
                    if (!rowNonPrime[c]) continue;

                    /* Check column sums (non‑prime) */
                    bool ok = true;
                    for (int col = 0; col < 3 && ok; ++col) {
                        int colSum = rowDigits[a][col] + rowDigits[b][col] + rowDigits[c][col];
                        if (isPrime[colSum]) ok = false;
                    }
                    if (!ok) continue;

                    /* Check diagonal sums (prime) */
                    int diag1 = rowDigits[a][0] + rowDigits[b][1] + rowDigits[c][2];
                    int diag2 = rowDigits[a][2] + rowDigits[b][1] + rowDigits[c][0];
                    if (!isPrime[diag1] || !isPrime[diag2]) continue;

                    trans_counts[state]++;
                }
            }
        }

        /* Allocate transition arrays */
        next_start = (int*)malloc((STATE + 1) * sizeof(int));
        next_start[0] = 0;
        for (int i = 0; i < STATE; ++i)
            next_start[i + 1] = next_start[i] + trans_counts[i];
        int totalTrans = next_start[STATE];

        next_to = (int*)malloc(totalTrans * sizeof(int));
        int *state_idx = (int*)calloc(STATE, sizeof(int));

        /* Store transitions */
        for (int a = 0; a < N_ROW; ++a) {
            if (!rowNonPrime[a]) continue;
            for (int b = 0; b < N_ROW; ++b) {
                if (!rowNonPrime[b]) continue;
                int state = a * N_ROW + b;
                for (int c = 0; c < N_ROW; ++c) {
                    if (!rowNonPrime[c]) continue;

                    bool ok = true;
                    for (int col = 0; col < 3 && ok; ++col) {
                        int colSum = rowDigits[a][col] + rowDigits[b][col] + rowDigits[c][col];
                        if (isPrime[colSum]) ok = false;
                    }
                    if (!ok) continue;

                    int diag1 = rowDigits[a][0] + rowDigits[b][1] + rowDigits[c][2];
                    int diag2 = rowDigits[a][2] + rowDigits[b][1] + rowDigits[c][0];
                    if (!isPrime[diag1] || !isPrime[diag2]) continue;

                    int pos = next_start[state] + state_idx[state];
                    next_to[pos] = c;
                    state_idx[state]++;
                }
            }
        }

        free(trans_counts);
        free(state_idx);
        initialized = true;
    }

    /* DP over rows: initialize with all admissible pairs */
    seq_t *cur  = dpA;
    seq_t *next = dpB;
    for (int i = 0; i < STATE; ++i) cur[i] = 0;
    for (int a = 0; a < N_ROW; ++a) {
        if (!rowNonPrime[a]) continue;
        for (int b = 0; b < N_ROW; ++b) {
            if (!rowNonPrime[b]) continue;
            cur[a * N_ROW + b] = 1;
        }
    }

    /* Extend DP by adding one row at a time */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATE; ++i) next[i] = 0;
        for (int state = 0; state < STATE; ++state) {
            seq_t val = cur[state];
            if (!val) continue;
            int start = next_start[state];
            int end   = next_start[state + 1];
            int last  = state % N_ROW;   /* last row of current pair */
            for (int k = start; k < end; ++k) {
                int c = next_to[k];
                int newState = last * N_ROW + c;
                next[newState] += val;
            }
        }
        /* Swap DP buffers */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum all possible final states */
    seq_t result = 0;
    for (int i = 0; i < STATE; ++i)
        result += cur[i];

    return result;
}
