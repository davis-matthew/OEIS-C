/*
 * A336528  a(1) = 1; a(2) = 2; for n > 2, a(n) is the least number > a(n-1)
 *          whose decimal representation is uniquely the concatenation of the
 *          decimal representations of two distinct earlier terms.
 * https://oeis.org/A336528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) is the least integer >a(n‑1) whose
 *            decimal representation is uniquely the concatenation of the
 *            decimal representations of two distinct earlier terms
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 586, u64 n <= 9208,
 *            u128 n <= 9208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A direct generator for A336528.  It keeps a global array of
    already computed terms and a global iterator that produces every
    positive integer containing only digits 1 and 2 in increasing
    numerical order.  For each candidate it counts the number of
    distinct splits into two earlier terms.  It accepts the candidate
    only if there is exactly one such split.  The implementation
    works for any seq_t and idx_t provided by the build.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global cache of the sequence.  */
static seq_t *SEQ = NULL;
static size_t SEQ_SIZE = 0;   /* number of terms currently stored */
static size_t SEQ_CAP  = 0;   /* allocated capacity */

/* Iterator over numbers that consist only of digits 1 and 2.  */
static size_t NEXT_LEN  = 1;  /* current number of digits being generated */
static size_t NEXT_MASK = 0;  /* current bit mask for this length */

/* ----------------------------------------------------------------------------
 *  Small helper: power of ten for small exponents (exponent < 64)
 * ------------------------------------------------------------------------ */
static inline seq_t pow10_local(idx_t exp)
{
    seq_t r = 1;
    while (exp--) r *= (seq_t)10;
    return r;
}

/* ----------------------------------------------------------------------------
 *  Append a value to the global sequence (growth strategy).
 * ------------------------------------------------------------------------ */
static void seq_add(seq_t v)
{
    if (SEQ_SIZE >= SEQ_CAP) {
        SEQ_CAP = SEQ_CAP ? SEQ_CAP * 2 : 16;
        SEQ = (seq_t *)realloc(SEQ, SEQ_CAP * sizeof(seq_t));
    }
    SEQ[SEQ_SIZE++] = v;
}

/* ----------------------------------------------------------------------------
 *  Binary search within the first 'upto' elements of SEQ.
 * ------------------------------------------------------------------------ */
static bool seq_exists(seq_t v, size_t upto)
{
    size_t lo = 0, hi = upto;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (SEQ[mid] < v)
            lo = mid + 1;
        else if (SEQ[mid] > v)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* ----------------------------------------------------------------------------
 *  Main sequence function.
 * ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; safeguard */

    /* Initialise cache on first call */
    if (SEQ_SIZE == 0) {
        SEQ_CAP = 16;
        SEQ = (seq_t *)malloc(SEQ_CAP * sizeof(seq_t));
        seq_add((seq_t)1);
        seq_add((seq_t)2);
    }

    /* Return already computed term, if available */
    if ((size_t)n <= SEQ_SIZE) return SEQ[(size_t)n - 1];

    /* Generate terms until we have at least n of them */
    while (SEQ_SIZE < (size_t)n) {
        /* Build next candidate consisting only of digits 1 and 2 */
        seq_t cand = 0;
        for (size_t i = 0; i < NEXT_LEN; ++i) {
            int bit = (NEXT_MASK >> (NEXT_LEN - 1 - i)) & 1;
            cand = cand * 10 + (bit ? (seq_t)2 : (seq_t)1);
        }

        /* Count unique splits into two earlier terms */
        int splits = 0;
        for (int split = 1; split < (int)NEXT_LEN && splits < 2; ++split) {
            seq_t pow = pow10_local((idx_t)(NEXT_LEN - split));
            seq_t left  = cand / pow;
            seq_t right = cand % pow;
            if (left == right) continue;                      /* must be distinct */
            if (seq_exists(left, SEQ_SIZE) && seq_exists(right, SEQ_SIZE))
                ++splits;
        }

        if (splits == 1)                   /* unique decomposition */
            seq_add(cand);

        /* Advance iterator to next number of digits 1 and 2 */
        NEXT_MASK++;
        if (NEXT_MASK == ((size_t)1 << NEXT_LEN)) {
            NEXT_MASK = 0;
            NEXT_LEN++;
        }
    }

    return SEQ[(size_t)n - 1];
}
