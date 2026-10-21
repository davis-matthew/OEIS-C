/*
 * A295064  Solution of the complementary equation a(n) = 8*a(n-3) + b(n-1),
 *          where a(0) = 1, a(1) = 3, a(2) = 5, b(0) = 2, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-3) + b(n-1) with a(0)=1, a(1)=3, a(2)=5, b(0)=2
 *            (a(n)) and (b(n)) are increasing complementary sequences
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static state: two growing arrays for a and b, the current number of
   filled terms (len), the allocated capacity (cap), and the last used
   integer (last_used). */
static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static size_t cap = 0;
static idx_t len = 0;
static seq_t last_used = 0;

/* Ensure that the two arrays can hold at least `need` elements. */
static void ensure_cap(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    b_seq = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Return 1 if `val` is present in the first `sz` elements of `arr`,
   which is assumed to be sorted increasingly. */
static int contains_a(const seq_t *arr, size_t sz, seq_t val)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return 1;
        if (arr[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* Generate a(n) and b(n) up to index `n`.  If they already exist,
   this is a no‑op; otherwise it appends new values. */
static void generate_to(idx_t n)
{
    if (n < len) return;                     /* already computed */

    size_t need = (size_t)(n + 1);
    ensure_cap(need);

    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
        if (i == 0)           /* initial values */
            val = 1;
        else if (i == 1)
            val = 3;
        else if (i == 2)
            val = 5;
        else                  /* recurrence */
            val = 8 * a_seq[i-3] + b_seq[i-1];

        a_seq[i] = val;          /* store a(i) */

        /* Find the smallest unused integer > last_used that is not in a[0..i]. */
        seq_t cand = last_used + 1;
        while (contains_a(a_seq, (size_t)(i+1), cand))
            cand++;             /* skip any a-value */
        b_seq[i] = cand;        /* store b(i) */
        last_used = cand;       /* update the last used number */
    }
    len = n + 1;
}

/* Public function required by the benchmark. */
seq_t a(idx_t n)
{
    generate_to(n);
    return a_seq[n];
}
