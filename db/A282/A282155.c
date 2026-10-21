/*
 * A282155  Number of nX2 0..2 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A282155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282155: Number of n×2 0..2 arrays with no element unequal to more
 * than four of its king-move neighbors, with the exception of exactly two
 * elements, and with new values introduced in order 0 sequentially upwards.
 *
 * The sequence satisfies the 12-term linear recurrence
 *
 *   a(n) = 21*a(n-1) -159*a(n-2) +559*a(n-3) -1332*a(n-4) +3408*a(n-5)
 *          -4888*a(n-6) +8256*a(n-7) -9024*a(n-8) +8512*a(n-9) -6912*a(n-10)
 *          +3072*a(n-11) -512*a(n-12)
 *
 * with initial values:
 *
 *   a(1)=0, a(2)=0, a(3)=1, a(4)=82, a(5)=1599, a(6)=20256, a(7)=217361,
 *   a(8)=2130206, a(9)=19642211, a(10)=173364188, a(11)=1480679925,
 *   a(12)=12326158938.
 *
 * The implementation memoises computed terms so that calls in arbitrary
 * order are efficient.
 */

#include <stdlib.h>

static seq_t *memo = NULL;   /* memoised a(1) .. a(memo_len) */
static size_t memo_len = 0;  /* number of terms stored (must be >= memo_cap) */
static size_t memo_cap = 0;  /* allocated capacity of memo */

/* Ensure that 'memo' has capacity at least 'need' elements */
static void
ensure_memo(size_t need)
{
    if (need <= memo_cap)
        return;
    size_t new_cap = memo_cap ? memo_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    /* C++ build: use 'new' and 'delete[]' */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < memo_cap; ++i)
        tmp[i] = memo[i];
    delete [] memo;
    memo = tmp;
#else
    /* C build: use realloc */
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();    /* out of memory */
    memo = tmp;
#endif
    memo_cap = new_cap;
}

/* Initialise memo with the first 12 known terms (once only) */
static void
init_memo(void)
{
    if (memo_len > 0)
        return;  /* already initialised */

    ensure_memo(12);

    /* a(1) .. a(12) */
    static const seq_t init_terms[13] = {
        (seq_t)0,          /* unused – index 0 */
        (seq_t)0,          /* a(1) */
        (seq_t)0,          /* a(2) */
        (seq_t)1,          /* a(3) */
        (seq_t)82,         /* a(4) */
        (seq_t)1599,       /* a(5) */
        (seq_t)20256,      /* a(6) */
        (seq_t)217361,     /* a(7) */
        (seq_t)2130206,    /* a(8) */
        (seq_t)19642211,   /* a(9) */
        (seq_t)173364188,  /* a(10) */
        (seq_t)1480679925, /* a(11) */
        (seq_t)12326158938/* a(12) */
    };

    for (size_t i = 0; i < 12; ++i)
        memo[i] = init_terms[i + 1];   /* store a(1) at memo[0] */

    memo_len = 12;
}

/* Coefficients of the recurrence (positive for odd k, negative for even k) */
static const seq_t coeffs[12] = {
    (seq_t)21, (seq_t)159, (seq_t)559, (seq_t)1332,
    (seq_t)3408, (seq_t)4888, (seq_t)8256, (seq_t)9024,
    (seq_t)8512, (seq_t)6912, (seq_t)3072, (seq_t)512
};

/* Public function computing a(n) for n >= 1 */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for indices < 1 */

    /* Convert idx_t to size_t for internal handling – safe for n >= 1 */
    size_t idx = (size_t)n;

    /* Allocate storage if required and initialise the base values */
    ensure_memo(idx);
    init_memo();

    /* Compute missing terms using the recurrence */
    if (idx > memo_len) {
        for (size_t i = memo_len; i < idx; ++i) {
            /* i corresponds to a(i+1) (0‑based) */
            seq_t acc = (seq_t)0;
            for (size_t k = 1; k <= 12; ++k) {
                seq_t prod = memo[i - k] * coeffs[k - 1];
                if (k & 1) {                   /* odd k – positive */
                    acc += prod;
                } else {                       /* even k – negative */
                    acc -= prod;
                }
            }
            memo[i] = acc;
        }
        memo_len = idx;
    }

    return memo[idx - 1];   /* a(n) is stored at memo[n-1] */
}
