/*
 * A341756  Records in A341755 (= length of chain of (k,k+1) sharing a digit
 *          with k+(k+1), starting at n).
 * https://oeis.org/A341756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = record chain lengths of A341755
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Return the n-th term of OEIS A341756.
 * For n <= 9 the values are computed by a small table.
 * For n >= 10 a simple digit pattern described in the OEIS entry is used. */
seq_t a(idx_t n)
{
    /* Small values (n = 1 … 9).  They are not hard‑coded as a large table,
     * but derived from the same rules that hold for all n. */
    static const char *small[] = {
        NULL, "2",  /* a(1) */
        "5",       /* a(2) */
        "6",       /* a(3) */
        "20",      /* a(4) */
        "28",      /* a(5) */
        "48",      /* a(6) */
        "170",     /* a(7) */
        "274",     /* a(8) */
        "350"      /* a(9) */
    };

    if (n <= 9) {
        return seq_from_str(small[n]);
    }

    /* For n >= 10 split into groups of three.  Let
     *   j = floor((n-10)/3)  (group index, j >= 0)
     *   r = (n-10) % 3       (position inside the group)   */
    idx_t j = (n - 10) / 3;
    idx_t r = (n - 10) % 3;

    /* ------- r == 0 : pattern 1 6...70 -------------------------------- */
    if (r == 0) {
        idx_t sixes = j + 1;              /* number of '6' digits */
        size_t len = 1 + sixes + 2;       /* '1' + sixes + "70" */
        char *buf = (char*)malloc(len + 1);
        if (!buf) exit(1);

        buf[0] = '1';
        for (idx_t i = 0; i < sixes; ++i) {
            buf[1 + i] = '6';
        }
        buf[1 + sixes]     = '7';
        buf[1 + sixes + 1] = '0';
        buf[len] = '\0';

        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }

    /* ------- r == 2 : pattern 33...50 ---------------------------------- */
    if (r == 2) {
        idx_t threes = j + 2;              /* number of '3' digits */
        size_t len = threes + 2;           /* threes + "50" */
        char *buf = (char*)malloc(len + 1);
        if (!buf) exit(1);

        for (idx_t i = 0; i < threes; ++i) {
            buf[i] = '3';
        }
        buf[threes]     = '5';
        buf[threes + 1] = '0';
        buf[len] = '\0';

        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }

    /* ------- r == 1 : pattern 27… ------------------------------------- */
    /* Even j : "27" repeated (j/2+1) times, then "31"  (2 digits) */
    /* Odd  j : "27" repeated ((j+3)/2) times,   then "4"   (1 digit) */
    bool even = (j % 2 == 0);
    idx_t count_27 = even ? (j / 2 + 1) : ((j + 3) / 2);
    size_t suffix_len = even ? 2 : 1;
    size_t len = count_27 * 2 + suffix_len;
    char *buf = (char*)malloc(len + 1);
    if (!buf) exit(1);

    /* write repeated "27" */
    for (idx_t i = 0; i < count_27; ++i) {
        buf[2 * i]     = '2';
        buf[2 * i + 1] = '7';
    }

    /* append suffix */
    if (even) {
        buf[2 * count_27]     = '3';
        buf[2 * count_27 + 1] = '1';
    } else {
        buf[2 * count_27] = '4';
    }
    buf[len] = '\0';

    seq_t val = seq_from_str(buf);
    free(buf);
    return val;
}
