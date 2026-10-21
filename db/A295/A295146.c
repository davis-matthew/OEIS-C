/*
 * A295146  Solution of the complementary equation a(n) = a(n-1) + 2*a(n-2) +
 *          b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) + b(n-2),
   a(0)=1, a(1)=3,
   b(0)=2, b(1)=4.
   (a(n)) and (b(n)) are increasing complementary sequences. */

#include <stdlib.h>
#include <stdbool.h>

/*--- Helper: binary search in increasing array -----------------------------------*/
static bool contains(const seq_t *arr, idx_t len, seq_t target)
{
    idx_t low = 0, high = len;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (arr[mid] == target) return true;
        if (arr[mid] < target)  low = mid + 1;
        else                    high = mid;
    }
    return false;
}

/*--- Helper: grow dynamic array -----------------------------------*/
static void ensure_capacity(idx_t *cap, idx_t need, seq_t **arr)
{
    if (*cap <= need) {
        idx_t new_cap = (*cap == 0 ? 4 : *cap);
        while (new_cap <= need) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, (size_t)new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/*--- Main sequence function -----------------------------------*/
seq_t a(idx_t n)
{
    /* Static state shared across calls */
    static seq_t *a_seq = NULL, *b_seq = NULL;
    static idx_t a_cap = 0, b_cap = 0;
    static idx_t len = 0;           /* number of computed a terms */
    static seq_t last_b = 0;        /* last computed value of b */

    /* ------------------------------------------------------------ */
    /* Initialise first two terms of each sequence on first call    */
    if (len == 0) {
        a_cap = 4;  b_cap = 4;
        a_seq = (seq_t*)malloc((size_t)a_cap * sizeof(seq_t));
        b_seq = (seq_t*)malloc((size_t)b_cap * sizeof(seq_t));
        a_seq[0] = (seq_t)1;  a_seq[1] = (seq_t)3;
        b_seq[0] = (seq_t)2;  b_seq[1] = (seq_t)4;
        len = 2;              /* we have a[0] and a[1] */
        last_b = (seq_t)4;    /* last b value used */
    }

    /* Return base cases immediately */
    if (n < len) return a_seq[n];

    /* ------------------------------------------------------------ */
    /* Extend tables up to required index n                         */
    for (idx_t i = len; i <= n; ++i) {
        /* Ensure enough space */
        ensure_capacity(&a_cap, i, &a_seq);
        ensure_capacity(&b_cap, i, &b_seq);

        /* Compute a[i] from recurrence */
        seq_t ai_1 = a_seq[i-1];
        seq_t ai_2 = a_seq[i-2];
        seq_t bi_2 = b_seq[i-2];
        a_seq[i] = ai_1 + (seq_t)2 * ai_2 + bi_2;

        /* Compute b[i] as the smallest integer > last_b not in a[0..i] */
        seq_t cand = last_b + (seq_t)1;
        while (contains(a_seq, i+1, cand)) {
            cand += (seq_t)1;
        }
        b_seq[i] = cand;
        last_b = cand;
    }

    len = n + 1;
    return a_seq[n];
}
