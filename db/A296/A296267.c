/*
 * A296267  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296267
   Complementary sequences a(n) and b(n) defined by
        a(0)=1, a(1)=3,
        b(0)=2, b(1)=4, b(2)=5,
        a(n)=a(n-1)+a(n-2)+b(n-2)*b(n)   (n>=2)
   with a and b increasing and partitioning the positive integers.
   The function returns a(n) for any n>=0.  It caches computed values
   to handle arbitrary calling order.  Values are stored in seq_t
   which may be of any unsigned integer type (fixed‑size or
   arbitrary‑precision).  No floating point or external libraries
   are used.  Time complexity is O(n). */
#include <stdlib.h>

static seq_t *a_vals   = NULL;   /* array for a(n) values           */
static seq_t *b_vals   = NULL;   /* array for b(n) values           */
static size_t a_cap   = 0;       /* allocated capacity of arrays    */
static size_t a_cnt   = 0;       /* number of a/b values stored    */
static size_t idx_b   = 0;       /* current index used when finding
                                    the next unused integer for b(n) */

/* Ensure the arrays have at least `need` slots; grow by doubling */
static void ensure_capacity(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 64;
    if (new_cap < need) new_cap = need;

    seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_a || !new_b) abort();

    for (size_t i = 0; i < a_cnt; ++i) {
        new_a[i] = a_vals[i];
        new_b[i] = b_vals[i];
    }
    free(a_vals);
    free(b_vals);
    a_vals = new_a;
    b_vals = new_b;
    a_cap  = new_cap;
}

/* Return a(n) for any index n >= 0 */
seq_t a(idx_t n)
{
    /* Initial construction on first call */
    if (a_vals == NULL) {
        ensure_capacity(3);            /* indices 0..2 */
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)3;
        b_vals[0] = (seq_t)2;
        b_vals[1] = (seq_t)4;
        b_vals[2] = (seq_t)5;
        a_vals[2] = a_vals[1] + a_vals[0] + b_vals[0] * b_vals[2];
        a_cnt   = 3;                  /* number of stored terms */
        idx_b   = 0;                  /* start scanning for b */
    }

    /* If the requested index has been computed, return it immediately */
    if ((size_t)n < a_cnt) return a_vals[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = a_cnt; i <= (size_t)n; ++i) {
        ensure_capacity(i + 1);

        /* Find next unused integer for b(i) */
        seq_t candidate = b_vals[i-1] + (seq_t)1;

        /* Skip over a-values that are < candidate */
        while (idx_b < i && a_vals[idx_b] < candidate) ++idx_b;

        /* If candidate equals an a-value, advance candidate */
        while (idx_b < i && a_vals[idx_b] == candidate) {
            ++candidate;
            ++idx_b;
        }

        b_vals[i] = candidate;

        /* Compute a(i) using the recurrence */
        a_vals[i] = a_vals[i-1] + a_vals[i-2] + b_vals[i-2] * b_vals[i];

        /* Update count of stored terms */
        a_cnt = i + 1;
    }

    return a_vals[(size_t)n];
}
