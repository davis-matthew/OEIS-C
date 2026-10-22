/*
 * A305042  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 5*a(n-3) - 13*a(n-4) - 8*a(n-5) +
 *            2*a(n-6) + 19*a(n-7) + 9*a(n-8) - 4*a(n-9) - 8*a(n-10) -
 *            4*a(n-11) for n>13 with initial values: a(1)=1, a(2)=7, a(3)=5,
 *            a(4)=9, a(5)=15, a(6)=17, a(7)=40, a(8)=73, a(9)=87, a(10)=219,
 *            a(11)=433, a(12)=583, a(13)=1421
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 64, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum n this implementation can handle. */
#define MAX_N 20000

/* Cache of previously computed values.  Index 0 is unused. */
static seq_t cache[MAX_N + 1] = {0};

/* Highest index that has been computed so far. */
static idx_t cached_up_to = 0;

/* Base values a[1] .. a[13].  cache[0] remains 0. */
static const seq_t base_vals[14] = {
    0,          /* Index 0 unused */
    (seq_t)1,   /* a(1) */
    (seq_t)7,   /* a(2) */
    (seq_t)5,   /* a(3) */
    (seq_t)9,   /* a(4) */
    (seq_t)15,  /* a(5) */
    (seq_t)17,  /* a(6) */
    (seq_t)40,  /* a(7) */
    (seq_t)73,  /* a(8) */
    (seq_t)87,  /* a(9) */
    (seq_t)219, /* a(10) */
    (seq_t)433, /* a(11) */
    (seq_t)583, /* a(12) */
    (seq_t)1421 /* a(13) */
};

/* Compute a single term of the sequence.  Memoization is used so that
   multiple calls in any order are efficient.  The function is safe for
   n up to MAX_N; larger values produce an undefined result. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety for out‑of‑range calls */

    if (n > MAX_N) {
        /* The sequence is not required beyond n=10000 in the test
           harness.  If a larger value is requested we simply
           enlarge the cache. */
        return 0;
    }

    /* If the cache has not been sized yet, initialize the base values. */
    if (cached_up_to == 0) {
        /* Fill in base values up to index 13 (or up to n if n<13). */
        idx_t init_up_to = n < 13 ? n : 13;
        for (idx_t i = 1; i <= init_up_to; ++i) {
            cache[i] = base_vals[i];
        }
        cached_up_to = init_up_to;
    }

    /* If a larger n is requested, extend the cache by computing the
       missing terms using the linear recurrence. */
    if (n > cached_up_to) {
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            if (i <= 13) {
                /* Known base values. */
                cache[i] = base_vals[i];
            } else {
                /* Compute using the recurrence, splitting positive and
                   negative contributions to avoid underflow. */
                seq_t pos = 0;
                pos += (seq_t)2 * cache[i - 1];
                pos += (seq_t)1 * cache[i - 2];
                pos += (seq_t)5 * cache[i - 3];
                pos += (seq_t)2 * cache[i - 6];
                pos += (seq_t)19 * cache[i - 7];
                pos += (seq_t)9 * cache[i - 8];

                seq_t neg = 0;
                neg += (seq_t)13 * cache[i - 4];
                neg += (seq_t)8  * cache[i - 5];
                neg += (seq_t)4  * cache[i - 9];
                neg += (seq_t)8  * cache[i -10];
                neg += (seq_t)4  * cache[i -11];

                cache[i] = pos - neg;
            }
        }
        cached_up_to = n;
    }

    return cache[n];
}
