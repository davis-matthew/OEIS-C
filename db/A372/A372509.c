/*
 * A372509  Numbers that can be written as a product of double factorials of
 *          odd numbers.
 * https://oeis.org/A372509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that can be written as a product of double
 *            factorials of odd numbers
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sorted sequence using a dynamic list of odd double factorials
   (3!! = 3, 5!! = 15, 7!! = 105, …) and a Hamming‑style algorithm. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic array for the first n terms */
    seq_t *seq = (seq_t *)malloc(n * sizeof(seq_t));
    if (!seq) return 0;          /* allocation failure – should not occur */
    seq[0] = 1;
    size_t seq_len = 1;           /* number of terms stored */

    /* Generator list: odd double factorials > 1 */
    #define MAX_GEN 256
    seq_t gen[MAX_GEN];           /* generator value */
    size_t idx[MAX_GEN];          /* current index into seq for this generator */
    seq_t cand[MAX_GEN];          /* next candidate value for this generator */
    size_t num_gen = 0;           /* number of generators in use */

    seq_t df = 1;                 /* current double factorial value (before multiplication) */
    seq_t nextOdd = 3;            /* next odd number to multiply to obtain the next double factorial */

    while (seq_len < n) {
        /* Ensure the first generator (3!!) is present */
        if (num_gen == 0) {
            seq_t newdf = df * nextOdd;          /* 1 * 3 = 3 */
            gen[num_gen] = newdf;
            idx[num_gen] = 0;
            cand[num_gen] = seq[0] * newdf;      /* = newdf */
            num_gen++;
            df = newdf;
            nextOdd += 2;
            continue;   /* reposition to find next minimum candidate */
        }

        /* Find the current smallest candidate among all generators */
        seq_t nextCand = cand[0];
        for (size_t i = 1; i < num_gen; ++i)
            if (cand[i] < nextCand) nextCand = cand[i];

        /* Add new generators whose double factorial does not exceed the current smallest candidate */
        while (1) {
            seq_t newdf = df * nextOdd;
            if (newdf <= nextCand) {
                if (num_gen >= MAX_GEN) break;   /* safety limit */
                gen[num_gen] = newdf;
                idx[num_gen] = 0;
                cand[num_gen] = seq[0] * newdf; /* candidate is the generator itself */
                num_gen++;
                df = newdf;
                nextOdd += 2;
                if (newdf < nextCand) nextCand = newdf;
            } else break;
        }

        /* Recompute the minimal candidate after possible additions */
        seq_t nextVal = cand[0];
        size_t min_i = 0;
        for (size_t i = 1; i < num_gen; ++i)
            if (cand[i] < nextVal) {
                nextVal = cand[i];
                min_i = i;
            }

        /* Append the smallest candidate to the sequence */
        seq[seq_len++] = nextVal;

        /* Advance all generators that produced this value */
        for (size_t i = 0; i < num_gen; ++i) {
            if (cand[i] == nextVal) {
                idx[i]++;
                cand[i] = seq[idx[i]] * gen[i];
            }
        }
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
