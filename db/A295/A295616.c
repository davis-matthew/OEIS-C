/*
 * A295616  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-3) +
 *          b(n-2), where a(0) = 1, a(1) = 2, a(2) = 3, b(0) = 4, b(1) = 5,
 *          b(2) = 6, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A295616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + b(n-2) (n>=3) a(0)=1, a(1)=2, a(2)=3;
 *            b(0)=4, b(1)=5, b(2)=6 a and b are increasing complementary
 *            sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------------  static storage  ----------------- */
static seq_t *a_seq = NULL;   /* a(0..a_size-1)           */
static seq_t *b_seq = NULL;   /* b(0..b_size-1)           */
static idx_t  a_size = 0;     /* number of a terms stored */
static idx_t  b_size = 0;     /* number of b terms stored */
static idx_t  a_cap  = 0;     /* current a allocation size  */
static idx_t  b_cap  = 0;     /* current b allocation size  */

/* -------------  helper: resize a_seq  ------------- */
static void ensure_a(idx_t n)
{
    if (n < a_cap) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 8;
    while (new_cap <= n) new_cap *= 2;
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* -------------  helper: resize b_seq  ------------- */
static void ensure_b(idx_t n)
{
    if (n < b_cap) return;
    idx_t new_cap = b_cap ? b_cap * 2 : 8;
    while (new_cap <= n) new_cap *= 2;
    b_seq = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

/* -------------  binary search in a_seq  ------------ */
static int a_contains(seq_t val, idx_t up_to)
{
    idx_t lo = 0, hi = up_to;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t midv = a_seq[mid];
        if (midv == val) return 1;
        else if (midv < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* -------------  initialisation of first 3 terms  ---- */
static void initialise(void)
{
    if (a_size > 0) return;           /* already done */

    /* --- a[0..2] --------------------------------- */
    ensure_a(2);
    a_seq[0] = 1;
    a_seq[1] = 2;
    a_seq[2] = 3;
    a_size = 3;

    /* --- compute b[0..2] as the complementary numbers */
    ensure_b(2);
    for (idx_t i = 0; i < 3; ++i) {
        seq_t cand = (i == 0) ? 1 : (b_seq[i-1] + 1);
        while (a_contains(cand, 2)) cand++;
        b_seq[i] = cand;
    }
    b_size = 3;
}

/* -------------   main entry function --------------- */
seq_t a(idx_t n)
{
    initialise();

    if (n < 3) {                     /* n=0,1,2 are known */
        return a_seq[n];
    }

    /* extend tables up to n (and corresponding b indices) */
    if (n >= a_size) {
        ensure_a(n);
        ensure_b(n);                 /* b indices up to n may be needed */

        /* generate missing terms */
        for (idx_t i = a_size; i <= n; ++i) {
            /* a[i] uses b[i-2] – already available because
               i-2 <= n-2 < a_size-1 once loop starts.                */
            seq_t an = 2 * a_seq[i-1] - a_seq[i-3] + b_seq[i-2];
            a_seq[i] = an;

            /* compute b[i] – smallest unused integer after b[i-1]    */
            seq_t cand = b_seq[i-1] + 1;
            while (a_contains(cand, i)) cand++;
            b_seq[i] = cand;
        }
        a_size = n + 1;
        b_size = n + 1;
    }

    return a_seq[n];
}
