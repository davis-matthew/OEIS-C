/*
 * A246888  Number of length n+4 0..4 arrays with some pair in every
 *          consecutive five terms totalling exactly 4.
 * https://oeis.org/A246888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 24, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A246888 – Number of length n+4 arrays over {0,1,2,3,4}
    such that every block of five consecutive terms contains at least
    one pair whose sum is 4.
    The implementation uses a 4‑digit DP over 5⁴ = 625 states.
    The recurrence is implicit: at each step we append a new digit
    and keep only those extensions that create a valid window
    of the last five digits.
    The initial step counts all 4‑digit suffixes (no window yet),
    then we perform n extensions to reach length n+4.
    Complexity:  O( n · 5⁴ · 5 )  ≈  O( n · 3 125 )  ≈ 5 000 000 operations
    for n = 10 000 – well within the time limit.  The algorithm
    works for any of the provided integer types (8‑, 16‑, 32‑,
    64‑ or 128‑bit, or arbitrary‑precision). */

#include <stdint.h>
#include <stdbool.h>

/* 5⁴ = 625 possible 4‑digit suffixes */
#define STATES 625
#define DIGITS 5

seq_t a(idx_t n)
{
    /* Transition tables, built once */
    static bool   allowed[STATES][DIGITS];
    static int    next_state[STATES][DIGITS];
    static bool   init_table = false;

    if (!init_table) {
        init_table = true;
        for (int s = 0; s < STATES; ++s) {
            int a0 = s / 125;              /* 5³ */
            int b0 = (s / 25) % 5;         /* 5² */
            int c0 = (s / 5) % 5;          /* 5¹ */
            int d0 = s % 5;                /* 5⁰ */
            for (int x = 0; x < DIGITS; ++x) {
                /* 5‑tuple (a0,b0,c0,d0,x) */
                int arr[5] = {a0, b0, c0, d0, x};
                bool ok = false;
                for (int i = 0; i < 5 && !ok; ++i)
                    for (int j = i + 1; j < 5; ++j)
                        if (arr[i] + arr[j] == 4) { ok = true; break; }
                allowed[s][x] = ok;
                next_state[s][x] = b0 * 125 + c0 * 25 + d0 * 5 + x;
            }
        }
    }

    /* DP arrays – current counts and next counts for each state.
       They are zeroed at each call. */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];

    /* Initialise for length 4: every 4‑digit suffix is possible */
    for (int s = 0; s < STATES; ++s) cur[s] = 1;

    /* Perform n extensions (to reach length n+4) */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int x = 0; x < DIGITS; ++x) {
                if (!allowed[s][x]) continue;
                int ns = next_state[s][x];
                nxt[ns] += val;
            }
        }
        /* Move next→cur and reset nxt */
        for (int s = 0; s < STATES; ++s) {
            cur[s] = nxt[s];
            nxt[s] = 0;
        }
    }

    /* Sum over all states to get a(n) */
    seq_t total = 0;
    for (int s = 0; s < STATES; ++s) total += cur[s];
    return total;
}
