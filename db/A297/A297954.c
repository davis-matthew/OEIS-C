/*
 * A297954  Number of n X 3 0..1 arrays with every element equal to 1, 2, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297954
 *  Recurrence: a(n) = 2*a(n-1)+2*a(n-2)+a(n-3)-8*a(n-4)-2*a(n-5)-2*a(n-6)+2*a(n-7)-a(n-8)+2*a(n-9)+3*a(n-11)+2*a(n-12)
 *  for n > 13, with initial values a(1)=1, a(2)=7, a(3)=15, a(4)=25, a(5)=47,
 *  a(6)=109, a(7)=245, a(8)=545, a(9)=1253, a(10)=2859, a(11)=6557,
 *  a(12)=15131, a(13)=34879.
 *
 *  The function below memoises the sequence up to a fixed maximum
 *  (here 20000, enough for the required 10 k tests).
 *  It works for any seq_t/idx_t pair provided by the build system.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N 20001   /* +1 to index n directly */

static seq_t cache[MAX_N];
static bool init_done = false;
static size_t computed_up_to = 0;   /* 0 means nothing computed yet */

/* Helper macro for array access */
#define CACHE(idx) cache[(size_t)(idx)]

seq_t a(idx_t n)
{
    /* Guard against indices < 1 – not part of the OEIS offset. */
    if (n < 1ULL) return (seq_t)0;

    /* Initialise the cache on first call */
    if (!init_done) {
        /* Zero initialise the entire array */
        for (size_t i = 0; i < MAX_N; ++i) cache[i] = (seq_t)0;

        /* Set the known initial values (indices 1..13) */
        CACHE(1)  = (seq_t)1;
        CACHE(2)  = (seq_t)7;
        CACHE(3)  = (seq_t)15;
        CACHE(4)  = (seq_t)25;
        CACHE(5)  = (seq_t)47;
        CACHE(6)  = (seq_t)109;
        CACHE(7)  = (seq_t)245;
        CACHE(8)  = (seq_t)545;
        CACHE(9)  = (seq_t)1253;
        CACHE(10) = (seq_t)2859;
        CACHE(11) = (seq_t)6557;
        CACHE(12) = (seq_t)15131;
        CACHE(13) = (seq_t)34879;

        computed_up_to = 13;
        init_done = true;
    }

    /* If n is within the already computed range, return immediately */
    if ((size_t)n <= computed_up_to) return CACHE(n);

    /* Compute missing terms up to n */
    for (size_t m = computed_up_to + 1; m <= (size_t)n; ++m) {
        /* Compute using the recurrence for m > 13 */
        seq_t val =
            CACHE(m-1)*2 + CACHE(m-2)*2 + CACHE(m-3)          /* +2a(n-1)+2a(n-2)+a(n-3) */
          + CACHE(m-7)*2                                       /* +2a(n-7) */
          + CACHE(m-9)*2                                       /* +2a(n-9) */
          + CACHE(m-11)*3                                      /* +3a(n-11) */
          + CACHE(m-12)*2;                                     /* +2a(n-12) */

        val -= CACHE(m-4)*8;       /* -8a(n-4) */
        val -= CACHE(m-5)*2;       /* -2a(n-5) */
        val -= CACHE(m-6)*2;       /* -2a(n-6) */
        val -= CACHE(m-8);         /* -a(n-8) */

        CACHE(m) = val;
    }

    computed_up_to = (size_t)n;
    return CACHE(n);
}
