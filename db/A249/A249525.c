/*
 * A249525  Number of length n+5 0..3 arrays with no six consecutive terms
 *          having five times any element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 29, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A249525: Number of length n+5 0..3 arrays with no six consecutive terms
   having five times any element equal to the sum of the remaining five.
   Computed by dynamic programming over the last 5 elements. 
   State space: all 5‑tuples over {0,1,2,3} (4^5 = 1024 states).
   Transitions are pre‑computed once; each step appends a new symbol
   and shifts the tuple left, rejecting transitions that create a forbidden
   six‑term window.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Transition table: nextState[s][c] gives the state after adding symbol c. */
static int  nextState[1024][4];
/* allowTrans[s][c] = true if the transition is allowed. */
static bool allowTrans[1024][4];
static bool transitions_built = false;

/* Build the transition tables; called once. */
static void build_transitions(void)
{
    for (int s = 0; s < 1024; ++s) {
        /* Decode state s into a 5‑tuple (t[0]..t[4]) in base‑4. */
        int tmp = s;
        int t[5];
        for (int i = 4; i >= 0; --i) {
            t[i] = tmp % 4;
            tmp /= 4;
        }

        /* Sum of the 5 elements in the current tuple. */
        int sum5 = t[0] + t[1] + t[2] + t[3] + t[4];

        for (int c = 0; c < 4; ++c) {
            int sum6 = sum5 + c;
            bool forbidden = false;

            if (sum6 % 6 == 0) {
                int val = sum6 / 6;          /* candidate element value */
                if (val <= 3) {              /* must be one of our symbols */
                    if (t[0] == val || t[1] == val || t[2] == val ||
                        t[3] == val || t[4] == val || c == val) {
                        forbidden = true;
                    }
                }
            }

            allowTrans[s][c] = !forbidden;

            /* Compute next state: drop first element, shift, append c. */
            int last4 = ((t[1] * 4 + t[2]) * 4 + t[3]) * 4 + t[4];
            nextState[s][c] = last4 * 4 + c;
        }
    }
    transitions_built = true;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (!transitions_built) {
        build_transitions();
    }

    /* The sequence is defined for n ≥ 1. Return 0 for n = 0 or negative. */
    if (n == 0) {
        return 0;
    }

    /* Length of the arrays to be counted: L = n + 5. */
    idx_t L = n + 5;
    /* Number of steps to append after the initial 5 symbols. */
    idx_t steps = L - 5;

    /* DP arrays: counts of sequences ending in each state. */
    seq_t cur[1024];
    seq_t next[1024];

    /* Initial count: each 5‑tuple can occur in exactly one way as the first 5 symbols. */
    for (int i = 0; i < 1024; ++i) {
        cur[i] = 1;
    }

    /* Perform the DP for each additional symbol. */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < 1024; ++i) {
            next[i] = 0;
        }
        for (int s = 0; s < 1024; ++s) {
            seq_t count = cur[s];
            if (count == 0) {
                continue;
            }
            for (int c = 0; c < 4; ++c) {
                if (!allowTrans[s][c]) {
                    continue;
                }
                int ns = nextState[s][c];
                next[ns] += count;
            }
        }
        /* Copy next into cur for the next iteration. */
        for (int i = 0; i < 1024; ++i) {
            cur[i] = next[i];
        }
    }

    /* Sum all counts: each ending state corresponds to a valid array. */
    seq_t result = 0;
    for (int i = 0; i < 1024; ++i) {
        result += cur[i];
    }

    return result;
}
