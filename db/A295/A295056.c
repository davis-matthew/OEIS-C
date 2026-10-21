/*
 * A295056  Solution of the complementary equation a(n) = 2*a(n-1) + b(n-1),
 *          where a(0) = 1, a(1) = 4, b(0) = 2, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + b(n-1); a(0)=1, a(1)=4, b(0)=2; b(n) = least
 *            positive integer not in {a(0)..a(k), b(0)..b(k-1)}. The
 *            implementation generates a and b sequentially, storing the a
 *            values in a static array and keeping a pointer that skips over
 *            previously seen a values when searching for the next b
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- static state for memoisation ---------- */
static seq_t *A          = NULL;   /* array of a(n) values */
static size_t A_cap      = 0;      /* allocated capacity of A   */
static size_t cur_len    = 0;      /* number of a values stored */
static idx_t  last_b     = 2;      /* last computed b value     */
static size_t ai         = 0;      /* index into A for b search */

/* ---------- utilities ---------- */
static void ensure_a_cap(size_t need)
{
    if (need <= A_cap) return;
    size_t newcap = A_cap ? A_cap * 2ULL : 16ULL;
    while (newcap < need) newcap *= 2ULL;
    seq_t *tmp = (seq_t*)realloc(A, newcap * sizeof(seq_t));
    A = tmp;
    A_cap = newcap;
}

static void init(void)
{
    ensure_a_cap(16ULL);
    A[0] = (seq_t)1;
    A[1] = (seq_t)4;
    cur_len = 2;
    last_b  = 2;
    ai      = 0;
}

/* ---------- required public function ---------- */
seq_t a(idx_t n)
{
    if (A == NULL) init();

    if ((size_t)n < cur_len) return A[(size_t)n];

    /* generate terms up to n */
    while (cur_len <= (size_t)n) {
        /* find next b (b(cur_len-1)) */
        idx_t candidate = last_b + 1ULL;

        /* skip over a values smaller than or equal to candidate */
        for (;;) {
            while (ai < cur_len && A[ai] < (seq_t)candidate)
                ai++;
            if (ai < cur_len && A[ai] == (seq_t)candidate) {
                /* candidate equals an 'a', skip it */
                candidate++;
                continue;
            }
            break;
        }
        seq_t b_val   = (seq_t)candidate;
        seq_t a_next  = (seq_t)2 * A[cur_len - 1] + b_val;

        ensure_a_cap(cur_len + 1);
        A[cur_len] = a_next;
        cur_len++;
        last_b = candidate;
    }

    return A[(size_t)n];
}
