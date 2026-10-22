/*
 * A301739  The number of trees with 4 nodes labeled by positive integers,
 *          where each tree's label sum is n.
 * https://oeis.org/A301739
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (C(n-1,3)+ (n even ? n/2-1 : 0))/2 // path tree orbits
 *            * + Σ_{t=3}^{n-1} ⌊(t^2+3)/12⌋ // star tree orbits * where
 *            C(n-1,3) = (n-1)(n-2)(n-3)/6. * The sum counts the unordered
 *            triples of positive integers that * sum to t, i.e. the number of
 *            partitions of t into exactly 3 parts
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, exit */

static seq_t *star_prefix = NULL;   /* star_prefix[i] = Π_{t=3}^{i} ⌊(t²+3)/12⌋ cumulative sum */
static idx_t  prefix_size = 0;     /* largest i that has been computed */

/* Ensure star_prefix is computed up to index `max_index` (inclusive). */
static void
ensure_star_prefix(idx_t max_index)
{
    if (max_index <= prefix_size)
        return;

    /* Allocate or grow the array. */
    seq_t *new_ptr = (seq_t *)realloc(star_prefix,
                                      (max_index + 1) * sizeof(seq_t));
    if (new_ptr == NULL)
        exit(EXIT_FAILURE);

    if (prefix_size == 0)          /* first allocation */
        new_ptr[0] = 0;

    /* Compute cumulative sums from the previous size + 1 up to max_index. */
    for (idx_t i = prefix_size + 1; i <= max_index; ++i) {
        seq_t p3 = 0;
        if (i >= 3) {
            seq_t t = (seq_t)i;
            p3 = (t * t + 3) / 12;    /* partitions of i into 3 parts */
        }
        new_ptr[i] = new_ptr[i - 1] + p3;
    }

    star_prefix = new_ptr;
    prefix_size = max_index;
}

/* Main sequence function. */
extern seq_t
a(idx_t n)
{
    if (n < 4)   /* offset is 4; behaviour below is irrelevant */
        return 0;

    /* ---- Path contribution ---- */
    seq_t T = ((seq_t)(n - 1) * (n - 2) * (n - 3)) / 6;           /* C(n-1,3)          */
    seq_t P = ((n & 1) == 0) ? ((seq_t)(n / 2) - 1) : 0;          /* palindromic sols   */
    seq_t path = (T + P) / 2;

    /* ---- Star contribution ---- */
    ensure_star_prefix(n - 1);
    seq_t star = star_prefix[n - 1];

    return path + star;
}
