/*
 * A390761  a(n) is the smallest positive integer k which has the same set of
 *          digits as n, and k is divisible by each nonzero digit of n, or -1
 *          if no such number exists.
 * https://oeis.org/A390761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = smallest positive integer k which has the same set * of
 *            digits as n and is divisible by each non‑zero digit of n, * or
 *            -1 if no such k exists
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 28, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_int.h"

#define STRIDE 2520           /* LCM of digits 1..9 */
#define MAX_MASK 1024         /* 2^10 possible digit sets */
#define MAX_STATE (MAX_MASK * STRIDE)

/* Helpers for small integer arithmetic */

/* Compute lcm of two integers, result fits in int (<=2520) */
static int lcm_int(int a, int b) {
    return a / gcd_int(a, b) * b;
}

/* Global structures reused for every evaluation to keep memory usage bounded */
static int parent_state[MAX_STATE];
static uint8_t parent_digit[MAX_STATE];
static unsigned int state_mark[MAX_STATE];
static int bfs_queue[MAX_STATE];
static unsigned int bfs_run = 1;          /* run counter to avoid clearing big arrays */

/* Cached results for each digit set mask */
static bool mask_computed[MAX_MASK] = {false};
static seq_t mask_cache[MAX_MASK];

/* ------------------------------------------------------------------ */
/* Main API ---------------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Build digit mask of n */
    int mask = 0;
    idx_t t = n;
    if (t == 0) {
        /* n should never be zero in the OEIS sequence */
        mask = 1;           /* digit '0' only */
    } else {
        while (t) {
            int d = t % 10;
            mask |= 1 << d;
            t /= 10;
        }
    }

    /* Quick impossibility test (contains 5, contains even digit, no zero) */
    if ((mask & 1) == 0) {                     /* no zero */
        if ((mask & (1 << 5))) {               /* has the digit 5  */
            int has_even = 0;
            if (mask & (1 << 2)) has_even = 1;
            if (mask & (1 << 4)) has_even = 1;
            if (mask & (1 << 6)) has_even = 1;
            if (mask & (1 << 8)) has_even = 1;
            if (has_even)
                return (seq_t)-1;
        }
    }

    /* Return cached result if available */
    if (mask_computed[mask])
        return mask_cache[mask];

    /* --------------------------------- */
    /* Prepare data for BFS over this mask */
    /* --------------------------------- */
    int full_mask = mask;
    /* Compute lcm of non‑zero digits in the set */
    int lcm = 1;
    for (int d = 1; d <= 9; ++d) {
        if (mask & (1 << d))
            lcm = lcm_int(lcm, d);
    }

    /* BFS structures */
    unsigned int curMark = bfs_run++;   /* new run marker */
    int head = 0, tail = 0;

    /* Push all possible first digits (non‑zero) */
    for (int d = 1; d <= 9; ++d) {
        if (!(mask & (1 << d))) continue;
        int rem = d % lcm;
        int state_idx = (1 << d) * STRIDE + rem;
        if (state_mark[state_idx] != curMark) {
            state_mark[state_idx] = curMark;
            parent_state[state_idx] = -1;
            parent_digit[state_idx] = (uint8_t)d;
            bfs_queue[tail++] = state_idx;
        }
    }

    /* BFS search */
    int solution_state = -1;
    while (head < tail) {
        int idx = bfs_queue[head++];
        int mask_used = idx / STRIDE;
        int rem = idx % STRIDE;

        /* Check if we have used all required digits */
        if (mask_used == full_mask) {
            bool ok = true;
            for (int d = 1; d <= 9; ++d) {
                if (mask & (1 << d)) {
                    if (rem % d != 0) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) {
                solution_state = idx;
                break;
            }
        }

        /* Expand with all allowed digits */
        for (int d = 0; d <= 9; ++d) {
            if (!(mask & (1 << d))) continue;
            int new_mask = mask_used | (1 << d);
            int new_rem = (rem * 10 + d) % lcm;
            int child_idx = new_mask * STRIDE + new_rem;
            if (state_mark[child_idx] != curMark) {
                state_mark[child_idx] = curMark;
                parent_state[child_idx] = idx;
                parent_digit[child_idx] = (uint8_t)d;
                bfs_queue[tail++] = child_idx;
            }
        }
    }

    /* If no solution found, store and return -1 */
    if (solution_state == -1) {
        mask_cache[mask] = (seq_t)-1;
        mask_computed[mask] = true;
        return (seq_t)-1;
    }

    /* Reconstruct the digits of the solution */
    uint8_t rev_digits[4096];
    int rev_len = 0;
    int cur = solution_state;
    while (cur != -1) {
        rev_digits[rev_len++] = parent_digit[cur];
        cur = parent_state[cur];
    }

    /* Build the result number */
    seq_t result = 0;
    for (int i = rev_len - 1; i >= 0; --i)
        result = result * 10 + rev_digits[i];

    /* Cache and return */
    mask_cache[mask] = result;
    mask_computed[mask] = true;
    return result;
}
