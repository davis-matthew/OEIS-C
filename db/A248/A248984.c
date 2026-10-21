/*
 * A248984  Number of length n+4 0..5 arrays with every five consecutive terms
 *          having two times the sum of some three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-5) -10*a(n-6) -31*a(n-10) +31*a(n-11)
 *            +26*a(n-15) -26*a(n-16) +8*a(n-20) -8*a(n-21) Initial values:
 *            a(1)=1326, a(2)=1654, a(3)=2042, a(4)=2494, a(5)=3014,
 *            a(6)=3606, … (up to a(22)=234886)
 * Range:     u8 none, u16 n <= 17, u32 n <= 57, u64 n <= 135, u128 n <= 210,
 *            big any n
 * Helpers:   idx_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/idx_max.h"

static seq_t base_arr[23] = {0};          /* base_arr[1..22] will hold a(1)..a(22) after init */
static bool base_computed = false;

/* helper to compute max of two idx_t values */

/* -------------------------------------------------------------------- */
/* Compute the first 22 terms by exhaustive dynamic programming over all */
/* possible 5‑tuples of digits 0..5.                                     */
/* -------------------------------------------------------------------- */
static void compute_base_22(void)
{
    /* good[tuple] == 1 if the 5‑tuple satisfies the required property   */
    static unsigned char good[7776];
    /* next_state[ state*6 + m ] = new_state if the extended 5‑tuple is
       good, otherwise -1                                         */
    static int next_state[7776];
    int i, j, k, l, m, idx, st, ns;

    /* Pre‑compute goodness of every 5‑tuple */
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 6; ++j)
            for (k = 0; k < 6; ++k)
                for (l = 0; l < 6; ++l)
                    for (m = 0; m < 6; ++m) {
                        int a = i, b = j, c = k, d = l, e = m;
                        int ok = 0;
                        if (2 * (a + b + c) == 3 * (d + e)) ok = 1;
                        else if (2 * (a + b + d) == 3 * (c + e)) ok = 1;
                        else if (2 * (a + b + e) == 3 * (c + d)) ok = 1;
                        else if (2 * (a + c + d) == 3 * (b + e)) ok = 1;
                        else if (2 * (a + c + e) == 3 * (b + d)) ok = 1;
                        else if (2 * (a + d + e) == 3 * (b + c)) ok = 1;
                        else if (2 * (b + c + d) == 3 * (a + e)) ok = 1;
                        else if (2 * (b + c + e) == 3 * (a + d)) ok = 1;
                        else if (2 * (b + d + e) == 3 * (a + c)) ok = 1;
                        else if (2 * (c + d + e) == 3 * (a + b)) ok = 1;
                        idx = i * 1296 + j * 216 + k * 36 + l * 6 + m;
                        good[idx] = (unsigned char)ok;
                    }

    /* Build transition table for a state consisting of the last four symbols */
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 6; ++j)
            for (k = 0; k < 6; ++k)
                for (l = 0; l < 6; ++l) {
                    st = i * 216 + j * 36 + k * 6 + l;   /* state = [i,j,k,l] */
                    for (m = 0; m < 6; ++m) {
                        idx = i * 1296 + j * 216 + k * 36 + l * 6 + m;
                        if (good[idx]) {
                            ns = j * 216 + k * 36 + l * 6 + m;  /* new state = [j,k,l,m] */
                            next_state[st * 6 + m] = ns;
                        } else {
                            next_state[st * 6 + m] = -1;
                        }
                    }
                }

    /* Dynamic programming over lengths */
    static seq_t dp_cur[1296];
    static seq_t dp_next[1296];
    int s, step;

    for (s = 0; s < 1296; ++s)
        dp_cur[s] = (seq_t)1;        /* any 4‑tuple is initially allowed */

    for (step = 1; step <= 22; ++step) { /* length = step + 4 */
        for (s = 0; s < 1296; ++s)
            dp_next[s] = (seq_t)0;

        for (s = 0; s < 1296; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            for (m = 0; m < 6; ++m) {
                ns = next_state[s * 6 + m];
                if (ns >= 0)
                    dp_next[ns] += val;
            }
        }

        /* Sum all states to obtain a(step) */
        seq_t sum = (seq_t)0;
        for (s = 0; s < 1296; ++s)
            sum += dp_next[s];
        base_arr[step] = sum;

        /* Prepare for next step */
        for (s = 0; s < 1296; ++s)
            dp_cur[s] = dp_next[s];
    }

    base_computed = true;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1 */

    /* Lazy allocation of dynamic arrays */
    static seq_t *arr = NULL;      /* a(1)..a(last_computed) */
    static seq_t *diff = NULL;     /* d(1)..d(last_computed) where d(i)=a(i)-a(i-1) */
    static size_t arr_cap = 0;
    static size_t diff_cap = 0;
    static idx_t last_computed = 0;

    /* Ensure capacity is large enough for n */
    if ((size_t)n + 1 > arr_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return (seq_t)0;   /* allocation failed */
        if (arr_cap) {
            /* Zero the newly allocated part */
            memset((void *)(tmp + arr_cap), 0,
                   (new_cap - arr_cap) * sizeof(seq_t));
        }
        arr = tmp;
        arr_cap = new_cap;
    }

    if ((size_t)n + 1 > diff_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(diff, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return (seq_t)0;
        if (diff_cap) {
            memset((void *)(tmp + diff_cap), 0,
                   (new_cap - diff_cap) * sizeof(seq_t));
        }
        diff = tmp;
        diff_cap = new_cap;
    }

    /* Compute base terms once */
    if (!base_computed)
        compute_base_22();

    /* Fill base array up to min(n,22) if necessary */
    if (last_computed < 22 && n >= 1) {
        idx_t limit = n < 22 ? n : 22;
        for (idx_t i = last_computed + 1; i <= limit; ++i)
            arr[i] = base_arr[i];

        /* Compute first differences for base indices */
        if (limit >= 1)
            diff[1] = arr[1];
        for (idx_t i = (idx_t)2; i <= limit; ++i)
            diff[i] = arr[i] - arr[i - 1];

        last_computed = limit;
    }

    /* If the requested index lies within the precomputed range, return it */
    if (n <= last_computed)
        return arr[n];

    /* Compute subsequent terms using the recurrence on d(n) */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t d_val = (seq_t)10 * diff[m - 5];
        d_val += (seq_t)26 * diff[m - 15];
        d_val += (seq_t)8  * diff[m - 20];
        d_val -= (seq_t)31 * diff[m - 10];

        diff[m] = d_val;
        arr[m] = arr[m - 1] + d_val;
    }

    last_computed = n;
    return arr[n];
}
