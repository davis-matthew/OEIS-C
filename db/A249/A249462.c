/*
 * A249462  Number of length n+4 0..4 arrays with no five consecutive terms
 *          having four times any element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for A249462 by dynamic programming over the last
   four elements.  For each 5‑tuple of consecutive terms the
   condition “four times the same element equals the sum of the
   remaining four” is checked; such a tuple is forbidden.
   The DP has 5⁴=625 states and runs in O(n) time, independent of
   the magnitude of the counting values.  */

#include <stdbool.h>
#include <stddef.h>

#define STATES 625          /* 5⁴ */
#define DIGITS 5            /* alphabet size */

static bool allowed[STATES][DIGITS] = {0};
static unsigned char a_arr[STATES], b_arr[STATES],
                     c_arr[STATES], d_arr[STATES];
static int tables_initialized = 0;

/* initialize the transition table once per process */
static void init_allowed(void) {
    if (tables_initialized) return;
    for (unsigned int s = 0; s < STATES; ++s) {
        unsigned int x = s;
        /* decode base‑5 state: a,b,c,d */
        unsigned int a = x / 125;          /* most significant digit */
        x = x % 125;
        unsigned int b = x / 25;
        unsigned int c = (x % 25) / 5;
        unsigned int d = x % 5;
        a_arr[s] = a; b_arr[s] = b; c_arr[s] = c; d_arr[s] = d;
        for (unsigned int e = 0; e < DIGITS; ++e) {
            unsigned int sum = a + b + c + d + e;
            bool forbidden = false;
            if (sum % 5 == 0) {
                unsigned int avg = sum / 5;
                if (a == avg || b == avg || c == avg || d == avg || e == avg)
                    forbidden = true;
            }
            allowed[s][e] = !forbidden;
        }
    }
    tables_initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is the offset index (a(1) corresponds to n=1) */
    idx_t steps = n;            /* number of extra elements to add after first 4 */
    idx_t L = n + 4;            /* total length, not actually used beyond steps */

    init_allowed();

    /* DP arrays */
    seq_t cur[STATES];
    seq_t nxt[STATES];

    /* start from any 4‑tuple */
    for (unsigned int s = 0; s < STATES; ++s) cur[s] = (seq_t)1;

    for (idx_t step = 0; step < steps; ++step) {
        /* reset next array */
        for (unsigned int s = 0; s < STATES; ++s) nxt[s] = (seq_t)0;

        for (unsigned int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            /* last three digits of state s: b,c,d */
            unsigned int last3 = (unsigned int)(s % 125);  /* b*25 + c*5 + d */
            for (unsigned int e = 0; e < DIGITS; ++e) {
                if (!allowed[s][e]) continue;
                unsigned int dst = last3 * 5 + e;
                nxt[dst] = nxt[dst] + val;
            }
        }
        /* copy next to current */
        for (unsigned int s = 0; s < STATES; ++s) cur[s] = nxt[s];
    }

    /* sum over all ending states */
    seq_t total = (seq_t)0;
    for (unsigned int s = 0; s < STATES; ++s) total = total + cur[s];
    return total;
}
