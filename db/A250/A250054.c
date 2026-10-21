/*
 * A250054  Number of length n+6 0..3 arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+6 arrays over {0,1,2,3} with no seven
 *            consecutive terms such that the maximum of some two terms equals
 *            the minimum of the remaining five terms
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 37, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  states of the last 6 symbols – 4^6 = 4096  */
#define STATE_COUNT 4096

/*  7‑symbol windows – 4^7 = 16384  */
#define WINDOW_COUNT 16384

/*  maximum n we are prepared to compute (covering the tests)  */
#define MAX_N_PREALLOC 20000

/*  the type definitions are supplied by the build system */
static unsigned char allowed[WINDOW_COUNT];          /* window admissibility  */
static seq_t curr[STATE_COUNT];                     /* counts for current length  */
static seq_t nxt[STATE_COUNT];                      /* counts for next length    */
static seq_t memo[MAX_N_PREALLOC + 1];              /* memoised a(n) values      */
static idx_t  max_n_computed = 0;                    /* largest n already computed*/
static bool initialized = false;                    /* do the one‑time init once  */

/*  -----------------------------------------------------  */
/*  One‑time initialisation: compute allowed array and
    initialise the DP base at length 6.   */
static void init_once(void)
{
    if (initialized) return;
    initialized = true;

    /*  compute whether each 7‑symbol window is allowed  */
    for (idx_t code = 0; code < WINDOW_COUNT; ++code) {
        int digits[7];
        idx_t tmp = code;
        for (int pos = 6; pos >= 0; --pos) {
            digits[pos] = tmp & 3;
            tmp >>= 2;
        }
        bool forbid = false;
        for (int i = 0; i < 7 && !forbid; ++i) {
            for (int j = i + 1; j < 7 && !forbid; ++j) {
                int m = (digits[i] > digits[j]) ? digits[i] : digits[j];
                int minOther = 4;   /* larger than any symbol   */
                for (int k = 0; k < 7; ++k) {
                    if (k == i || k == j) continue;
                    if (digits[k] < minOther) minOther = digits[k];
                }
                if (m == minOther) forbid = true;
            }
        }
        allowed[code] = !forbid;
    }

    /*  initial DP for sequences of length 6 – all suffixes are reachable  */
    for (idx_t s = 0; s < STATE_COUNT; ++s)
        curr[s] = (seq_t)1;
    /*  memo[0] is unused; max_n_computed stays 0  */
}

/*  -----------------------------------------------------  */
/*  Extend the DP to compute a(n) up to the requested n      */
static void compute_up_to_n(idx_t target_n)
{
    if (target_n <= max_n_computed) return;

    /*  current length processed by curr:   len = max_n_computed + 6  */
    while (max_n_computed < target_n) {
        /*  zero next array  */
        for (idx_t j = 0; j < STATE_COUNT; ++j)
            nxt[j] = (seq_t)0;

        /*  perform transitions  */
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = curr[s];
            if (cnt == (seq_t)0) continue;
            idx_t base = s << 2;          /* s*4   */
            for (int c = 0; c < 4; ++c) {
                if (!allowed[base + c]) continue;
                idx_t t = ((s & 1023) << 2) | (idx_t)c;   /* drop oldest digit */
                nxt[t] += cnt;
            }
        }

        /*  sum counts to get a(new_n)  */
        seq_t total = (seq_t)0;
        for (idx_t j = 0; j < STATE_COUNT; ++j)
            total += nxt[j];

        idx_t new_n = max_n_computed + 1;
        memo[new_n] = total;

        /*  prepare for next step  */
        for (idx_t j = 0; j < STATE_COUNT; ++j)
            curr[j] = nxt[j];
        max_n_computed = new_n;
    }
}

/*  -----------------------------------------------------  */
/*  Public function required by the specification           */
seq_t a(idx_t n)
{
    init_once();                /* one‑time init */
    if (n == 0) return (seq_t)0;   /* undefined for offset=1, but harmless  */
    compute_up_to_n(n);
    return memo[n];
}
