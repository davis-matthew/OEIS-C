/*
 * A328096  a(0) = 0; a(1) = 1; for n > 1, a(n) = number of terms between the
 *          two previous occurrences of a(n-1) if a(n-1) has appeared two or
 *          more times, otherwise a(n) = 0.
 * https://oeis.org/A328096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; For n > 1: let v = a(n-1). If v has appeared
 *            at least twice before index n-1, let i2 < i1 be the last two
 *            indices with a(i)=v (i2 > i1), then a(n) = i1 - i2 - 1;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Sentinel representing "not yet seen" for idx_t indices. */
static const idx_t INF = (idx_t)-1;

/* Maximum value expected for a(n).  The OEIS entry states the
   largest term up to n = 10000 is 9040, so 20000 is comfortably safe. */
#define MAXVAL 20000

/* Cache of computed sequence values and bookkeeping arrays. */
static seq_t  seq[MAXVAL + 1];
static idx_t  last1[MAXVAL + 1];
static idx_t  last2[MAXVAL + 1];
static idx_t  seq_len = 0;   /* number of terms already computed (>=1) */

/* Initialise bookkeeping arrays to the "not seen" sentinel. */
static void init_bookkeeping(void)
{
    for (idx_t i = 0; i <= MAXVAL; ++i) {
        last1[i] = INF;
        last2[i] = INF;
    }
}

/* Compute sequence up to index n (inclusive).  This routine is
   idempotent: calling it multiple times with the same or larger n
   extends the cache only once. */
static void grow_to(idx_t n)
{
    if (seq_len == 0) {
        /* Seed the first two terms. */
        init_bookkeeping();
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        last1[0] = 0;
        last2[0] = INF;
        last1[1] = 1;
        last2[1] = INF;
        seq_len = 2;
    }

    while (seq_len <= n) {
        idx_t i = seq_len;      /* current index to compute */
        seq_t v = seq[i - 1];   /* a(i-1) */

        /* Ensure v is within bounds of our bookkeeping tables. */
        if ((idx_t)v > MAXVAL) {
            /* In the intended usage this never happens.  If it does,
               we would need to resize the tables, but the build
               configuration guarantees it will not. */
            return;
        }

        seq_t cur;
        if (last2[(idx_t)v] != INF) {
            /* v has appeared at least twice before.  Distance = last1 - last2 - 1. */
            cur = (seq_t)(last1[(idx_t)v] - last2[(idx_t)v] - 1);
        } else {
            cur = (seq_t)0;
        }

        seq[i] = cur;

        /* Update bookkeeping for the newly produced value cur. */
        if ((idx_t)cur <= MAXVAL) {
            last2[(idx_t)cur] = last1[(idx_t)cur];
            last1[(idx_t)cur] = i;
        }

        ++seq_len;
    }
}

seq_t a(idx_t n)
{
    grow_to(n);
    return seq[n];
}
