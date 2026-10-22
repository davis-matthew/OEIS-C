/*
 * A328727  Nonnegative numbers whose base-3 expansion has no two consecutive
 *          nonzero digits.
 * https://oeis.org/A328727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th non‑negative integer whose base‑3 * expansion
 *            contains no two consecutive digits that are non‑zero. * The
 *            terms are listed in increasing numerical order starting with *
 *            a(1)=0. This implementation enumerates the admissible base‑3 *
 *            strings in length‑increasing order and uses unra...
 * Range:     u8 n <= 51, u16 n <= 1706, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ----------  Pre‑computed tables  ---------- */

/* We pre‑allocate a fixed amount of space.  For n up to about
 * 10 000 the required base‑3 length is < 20, so 64 is plenty. */
#define MAXLEN 64

static seq_t b_arr[MAXLEN + 1];    /* b[i] = number of length‑i strings
                                      over {0,1,2} with no two
                                      consecutive non‑zeros.  b[0]=1, b[1]=3. */
static seq_t pow3_arr[MAXLEN + 1]; /* pow3_arr[i] = 3^i. */

static bool tables_inited = false;

/* Initialise the tables on first use. */
static void init_tables(void)
{
    if (tables_inited)
        return;

    b_arr[0] = 1;
    b_arr[1] = 3;
    for (size_t i = 2; i <= MAXLEN; ++i)
        b_arr[i] = b_arr[i - 1] + (seq_t)2 * b_arr[i - 2];

    pow3_arr[0] = 1;
    for (size_t i = 1; i <= MAXLEN; ++i)
        pow3_arr[i] = pow3_arr[i - 1] * 3;

    tables_inited = true;
}

/* ----------  Unranking of a suffix of length len  ---------- */

/* Return the rank‑th (0‑based) string of length 'len' that satisfies
 * the adjacency rule and is preceded by a zero digit.  The digit
 * before the first of these len digits is assumed to be zero.
 * This function is used to build the lower L‑2 digits of a number
 * of total length L (where L≥2) after the leading '1' or '2' and
 * the forced middle 0 have already been chosen. */
static seq_t suffix_value(idx_t rank, size_t len)
{
    seq_t value = 0;
    bool prev_nonzero = false;

    for (size_t pos = 0; pos < len; ++pos) {
        size_t remaining = len - pos;
        int digit;

        if (prev_nonzero) {            /* previous digit was non‑zero → must pick 0 */
            digit = 0;
        } else {                       /* previous digit was zero → 0,1,2 possible */
            idx_t cnt0 = (remaining >= 1) ? (idx_t)b_arr[remaining - 1] : 1;
            if (rank < cnt0) {
                digit = 0;
            } else {
                rank -= cnt0;
                idx_t cnt1 = (remaining >= 2) ? (idx_t)b_arr[remaining - 2] : 1;
                if (rank < cnt1) {
                    digit = 1;
                } else {
                    rank -= cnt1;
                    digit = 2;
                }
            }
        }

        value = value * 3 + digit;
        prev_nonzero = (digit != 0);
    }

    return value;
}

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    init_tables();

    /* Offset: a(1) = 0 (the only even number of the form). */
    if (n == 1)
        return 0;

    /* For n ≥ 2 we work with the (n‑2)-th positive number. */
    idx_t target = n - 2;               /* 0‑based rank among positive terms */

    /* Find the length L of the base‑3 representation that contains
       this target.  Count of admissible numbers of length L:
       count(L) = 2 (L=1), 2 * b_arr[L‑2] (L≥2).               */
    size_t L = 1;
    idx_t cum = 0;                      /* sum of counts for lengths < L   */

    while (true) {
        idx_t count;
        if (L == 1)
            count = 2;
        else
            count = (idx_t)((seq_t)2 * b_arr[L - 2]);

        if (target < cum + count)
            break;
        cum += count;
        ++L;
    }

    idx_t rank = target - cum;           /* 0‑based rank within length L */

    /* Length 1: numbers 1 or 2. */
    if (L == 1) {
        return (seq_t)(rank + 1);        /* 1 or 2 */
    }

    /* For L ≥ 2 the most significant digit is 1 or 2,
       the second digit is forced to 0. */
    idx_t split = (idx_t)b_arr[L - 2];    /* number of strings with MSB 1 */
    seq_t msb = (rank < split) ? 1 : 2;
    if (rank >= split)
        rank -= split;

    seq_t suffix = suffix_value(rank, L - 2);
    seq_t result = msb * pow3_arr[L - 1] + suffix;
    return result;
}
