/*
 * A240974  Numbers of the form x^y - x*y for x > 1 and y > 1.
 * https://oeis.org/A240974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 98, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, pow_minus_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240974: Numbers of the form x^y - x*y for integers x>1,y>1,
   sorted ascending, without duplicates.  This implementation enumerates
   all pairs (x,y) producing values <= LIMIT, stores them, sorts, and
   removes duplicates.  The resulting array is cached for subsequent calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/pow_minus_xy.h"

/* Comparator for qsort */
static int seq_cmp(const void *a, const void *b)
{
    seq_t A = *(const seq_t *)a;
    seq_t B = *(const seq_t *)b;
    if (A < B) return -1;
    if (A > B) return  1;
    return 0;
}

/* Static storage for the generated sequence */
static seq_t *SEQ_VALUES   = NULL;
static size_t  SEQ_LEN     = 0;
static size_t  SEQ_CAP     = 0;
static bool    SEQ_READY   = false;

/* Build the sequence once */
static void build_sequence(void)
{
    /* Upper bound for generated values – suffices for the first ~10⁴ terms */
    const seq_t LIMIT = (seq_t)1000000000000ULL;   /* 1 000 000 000 000 */

    for (uint64_t x = 2; ; ++x)
    {
        /* If even x² is larger than LIMIT, all further x produce values > LIMIT */
        if ((unsigned __int128)x * x > (unsigned __int128)LIMIT)
            break;

        for (uint64_t y = 2; ; ++y)
        {
            seq_t val = pow_minus_xy(x, y, LIMIT);
            if (val > LIMIT)
                break;
            append_candidate(&SEQ_VALUES, &SEQ_LEN, &SEQ_CAP, val);
        }
    }

    /* Sort and remove duplicates */
    qsort(SEQ_VALUES, SEQ_LEN, sizeof(seq_t), seq_cmp);

    size_t uniq = 0;
    for (size_t i = 0; i < SEQ_LEN; ++i)
    {
        if (uniq == 0 || SEQ_VALUES[i] != SEQ_VALUES[uniq - 1])
            SEQ_VALUES[uniq++] = SEQ_VALUES[i];
    }
    SEQ_LEN = uniq;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!SEQ_READY)
    {
        build_sequence();
        SEQ_READY = true;
    }
    return SEQ_VALUES[(size_t)n - 1];
}
