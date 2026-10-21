/*
 * A293316  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + 1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + 1 with a(0)=1, a(1)=3, b(0)=2,
 *            b(1)=4. Here b(k) denotes the k‑th positive integer not
 *            belonging to the set {a(0),…,a(k)}. The function builds a(n) in
 *            O(n log n) time using a Fenwick tree to answer “k‑th missing
 *            integer” queries
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Fenwick tree helpers ----------------------------------------------------- */
static void bit_update(int *bit, int size, int idx, int delta)
{
    for (; idx <= size; idx += idx & -idx)
        bit[idx] += delta;
}

/* Returns the 1‑based index of the (k‑th, 0‑based) missing integer
   among the numbers 1..size.  The tree initially contains 1 at every
   position, and positions that become “used” are decreased by 1. */
static int bit_find_kth(int *bit, int size, int k)
{
    int target = k + 1;                /* 1‑based rank we want */
    int idx = 0;
    int mask = 1;
    while (mask <= size) mask <<= 1;
    mask >>= 1;                        /* largest power of two ≤ size */

    for (; mask; mask >>= 1) {
        int next = idx + mask;
        if (next <= size && bit[next] < target) {
            idx = next;
            target -= bit[next];
        }
    }
    return idx + 1;                    /* 1‑based position */
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* handle the two base terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* allocate Fenwick tree for indices up to a reasonable limit */
    int max_limit = (int)(n * 2 + 100);     /* safe upper bound for b(n) */
    int size = max_limit;
    int *bit = (int *)malloc((size + 1) * sizeof(int));
    if (!bit) return (seq_t)0;              /* out of memory – unlikely */

    /* initialise tree with all 1s (every number is initially missing) */
    for (int i = 1; i <= size; ++i)
        bit[i] = i & -i;                    /* classic Fenwick initialisation */

    /* mark the two first a-values as used; they are 1 and 3 */
    if (1 <= size) bit_update(bit, size, 1, -1);
    if (3 <= size) bit_update(bit, size, 3, -1);

    /* iterate, keeping only the last two a-values */
    seq_t a_prev2 = (seq_t)1;              /* a(0) */
    seq_t a_prev  = (seq_t)3;              /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* b(i-2) : the (i-2)-th missing integer */
        int b_int = bit_find_kth(bit, size, (int)(i - 2));
        seq_t b_val = (seq_t)b_int;

        seq_t a_curr = a_prev + a_prev2 + b_val + (seq_t)1;

        /* if a_curr does not exceed our lookup limit, mark it as used */
        if (a_curr <= (seq_t)size) {
            int pos = (int)a_curr;          /* safe cast: a_curr ≤ size */
            bit_update(bit, size, pos, -1);
        }

        a_prev2 = a_prev;
        a_prev  = a_curr;
    }

    free(bit);
    return a_prev;
}
