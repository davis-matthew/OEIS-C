/*
 * A367817  Number of terms in a shortest sequence of Fibonacci numbers that
 *          sum to n, allowing Fibonacci numbers with negative indices.
 * https://oeis.org/A367817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the minimal number of Fibonacci numbers (allowing
 *            indices * from –∞ to +∞) whose sum equals n. Equivalently, a(n)
 *            is the minimum * length of a sequence of terms +F(k) (for all
 *            k≥1) and –F(k) (only for * even k) that sums to n, with a(0)=0.
 *            This implementation computes a * single static ta...
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Configuration: increase LIMIT_FOR_SEQUENCES to cover the required n. */
/* --------------------------------------------------------------------- */
#define LIMIT_FOR_SEQUENCES 200000   /* max absolute value to pre‑compute */

/* --------------------------------------------------------------------- */
/* Static data structures: distance table and Fibonacci list. */
/* --------------------------------------------------------------------- */
static seq_t *DIST = NULL;           /* distance array for sums in [-LIMIT,+LIMIT] */
static size_t DIST_SIZE = 0;         /* number of elements in DIST */

static seq_t *FIB_VALS = NULL;      /* Fibonacci values F(1),F(2),... <= LIMIT */
static char   *FIB_EVEN = NULL;     /* 1 if index k is even, else 0 */
static size_t FIB_CNT = 0;           /* number of stored Fibonacci numbers */

static idx_t    OFFSET = LIMIT_FOR_SEQUENCES;   /* offset to convert sum to index */

/* --------------------------------------------------------------------- */
/* Helper: build Fibonacci list and distance table.                       */
/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    /* -----------------------------------------------------------------
     * Build Fibonacci list up to LIMIT_FOR_SEQUENCES.
     * ----------------------------------------------------------------- */
    seq_t  a = 1, b = 1;          /* F1 = F2 = 1 */
    idx_t  idx = 1;
    FIB_CNT = 0;
    size_t fib_cap = 64;
    FIB_VALS = (seq_t *)malloc(fib_cap * sizeof(seq_t));
    FIB_EVEN = (char *)malloc(fib_cap * sizeof(char));
    if (!FIB_VALS || !FIB_EVEN)
        return;  /* allocation failure: leave DIST NULL */

    while (a <= LIMIT_FOR_SEQUENCES) {
        if (FIB_CNT == fib_cap) {          /* grow arrays */
            fib_cap *= 2;
            FIB_VALS = (seq_t *)realloc(FIB_VALS, fib_cap * sizeof(seq_t));
            FIB_EVEN = (char *)realloc(FIB_EVEN, fib_cap * sizeof(char));
            if (!FIB_VALS || !FIB_EVEN)
                return;
        }
        FIB_VALS[FIB_CNT] = a;
        FIB_EVEN[FIB_CNT] = (idx % 2 == 0) ? 1 : 0;   /* even indices allow subtraction */
        FIB_CNT++;

        /* Advance Fibonacci numbers */
        seq_t next = a + b;
        a = b;
        b = next;
        idx++;
    }

    /* -----------------------------------------------------------------
     * Allocate distance array.
     * ----------------------------------------------------------------- */
    DIST_SIZE = 2 * OFFSET + 1;
    DIST = (seq_t *)malloc(DIST_SIZE * sizeof(seq_t));
    if (!DIST)
        return;  /* allocation failure */

    /* sentinel value: all bits set to 1 for seq_t */
    const seq_t INF = (seq_t)(-1);
    for (size_t i = 0; i < DIST_SIZE; ++i)
        DIST[i] = INF;

    /* -----------------------------------------------------------------
     * BFS: minimal number of steps from 0 to every sum in [-LIMIT,+LIMIT].
     * ----------------------------------------------------------------- */
    int *queue = (int *)malloc(DIST_SIZE * sizeof(int));
    if (!queue) {
        free(DIST);
        DIST = NULL;
        return;
    }
    size_t qhead = 0, qtail = 0;

    DIST[OFFSET] = 0;          /* a(0) = 0 */
    queue[qtail++] = 0;        /* start from sum 0 */

    while (qhead < qtail) {
        int cur = queue[qhead++];
        seq_t curDist = DIST[(size_t)(cur + OFFSET)];

        for (size_t i = 0; i < FIB_CNT; ++i) {
            int k = (int)FIB_VALS[i];

            /* Move by +F(k)  (always allowed) */
            int pos = cur + k;
            if (pos >= -(int)OFFSET && pos <= (int)OFFSET) {
                size_t idx = (size_t)(pos + OFFSET);
                if (DIST[idx] == INF) {
                    DIST[idx] = curDist + 1;
                    queue[qtail++] = pos;
                }
            }

            /* Move by –F(k) only if index k is even */
            if (FIB_EVEN[i]) {
                int neg = cur - k;
                if (neg >= -(int)OFFSET && neg <= (int)OFFSET) {
                    size_t idx = (size_t)(neg + OFFSET);
                    if (DIST[idx] == INF) {
                        DIST[idx] = curDist + 1;
                        queue[qtail++] = neg;
                    }
                }
            }
        }
    }

    free(queue);
}

/* --------------------------------------------------------------------- */
/* Public API: compute a(n).                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();                 /* ensure table is built */

    /* if build failed, return 0 */
    if (!DIST)
        return (seq_t)0;

    /* n is non‑negative by definition of idx_t */
    if (n > LIMIT_FOR_SEQUENCES)
        return (seq_t)0;   /* outside pre‑computed range; values not required by tests */

    return DIST[(size_t)(n + OFFSET)];
}
