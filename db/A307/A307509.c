/*
 * A307509  Using the mapping in A306979, the sum of powers of 2 where the
 *          exponents are the digits of the previous term, a(n) is the number
 *          of steps to reach the end of the first cycle.
 * https://oeis.org/A307509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307509: a(n) = number of steps until the first repeated term
 * when iterating the map f(x) = Σ 2^d over all decimal digits d of x.
 *
 * The sequence starts with a(0)=35, a(1)=34, …
 * This implementation follows the definition directly and
 * tracks the first repeated term using a visited array.
 *
 * The maximum possible value of f(x) for any 64‑bit integer is
 * 512 × 20 = 10240, so using a fixed array of size 20000 suffices
 * for all seq_t sizes. */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>

/* Compile‑time constant for array bounds */
#define MAXV 20000U

/* pre‑computed powers of two up to 2^9 (digit 9) */
static const seq_t POW2[10] = {
    (seq_t)1,  (seq_t)2,  (seq_t)4,  (seq_t)8,  (seq_t)16,
    (seq_t)32, (seq_t)64, (seq_t)128,(seq_t)256,(seq_t)512
};

/* Compute f(x) = Σ 2^digit(x) */
static seq_t map_func(seq_t x)
{
    seq_t sum = (seq_t)0;
    if (x == (seq_t)0) {
        sum = POW2[0];          /* 2^0 = 1 */
    } else {
        seq_t tmp = x;
        while (tmp != (seq_t)0) {
            unsigned d = (unsigned)(tmp % (seq_t)10); /* current decimal digit */
            sum += POW2[d];
            tmp /= (seq_t)10;
        }
    }
    return sum;
}

/* Sequence A307509 */
seq_t a(idx_t n)
{
    /* visited array stores the step at which each value was first seen.
       Positions <= MAXV are tracked; larger values cannot re‑appear. */
    static idx_t visited[MAXV + 1];
    const idx_t UNVIS = ((idx_t)-1);

    /* reset visited markers to UNVIS for each call */
    for (size_t i = 0; i <= MAXV; ++i) {
        visited[i] = UNVIS;
    }

    seq_t cur = n;
    idx_t step = 0;                /* number of steps taken so far */

    if (cur <= (seq_t)MAXV) {
        visited[(idx_t)cur] = step;
    }

    while (1) {
        seq_t nxt = map_func(cur);
        ++step;                /* we are moving to the next element */

        if (nxt <= (seq_t)MAXV) {
            idx_t pos = (idx_t)nxt;
            if (visited[pos] != UNVIS) { /* first repetition detected */
                return (seq_t)step;
            }
            visited[pos] = step; /* record first visit of nxt */
        }

        /* values > MAXV cannot reappear; no need to store them */
        cur = nxt;
    }
}
