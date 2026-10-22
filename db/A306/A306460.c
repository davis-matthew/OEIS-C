/*
 * A306460  Number of ways to write n as x*(2x-1) + y*(y+1)/2 +
 *          z*(z+1)*(z+2)/6, where x,y,z are nonnegative integers with x > 0.
 * https://oeis.org/A306460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as x*(2x-1) + y*(y+1)/2 +
 *            z*(z+1)*(z+2)/6 with x > 0, y,z ≥ 0
 * Range:     u8 n <= 548, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation precomputes
   the number of ways to write an integer as
   a triangular number plus a tetrahedral number
   (triangular+tetra) into a table.  Then for
   each n it sums over all possible hexagonal
   parts x*(2x-1).  The table is grown lazily
   so that repeated calls are cheap.  The
   algorithm runs in O(sqrt(n)) time for each
   query and needs only O(n) space. */

#include <stdlib.h>

/* Static storage for the pre–computed counts
   of triangular+tetrahedral sums.  The array
   is indexed by sum value and holds a seq_t
   count. */
static seq_t *tritetra_counts = NULL;
static idx_t   tritetra_maxsize = 0;   /* highest index that has been computed */

/* Ensure that tritetra_counts contains counts for
   all sums up to `needed`.  Rebuilds the table if
   a larger size is requested. */
static void ensure_tritetra_counts(idx_t needed)
{
    if (tritetra_counts && tritetra_maxsize >= needed)
        return;   /* already sufficient */

    /* Allocate a fresh table up to needed (inclusive) and
       zero‑initialise it.  We rebuild the entire table
       each time we increase the size to keep the code simple. */
    size_t  new_size = (size_t)(needed + 1);
    seq_t *new_counts = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_counts)
        return;  /* allocation failure; subsequent calls will fail */

    /* Enumerate all triangular numbers T(y) and all
       tetrahedral numbers Te(z) that are ≤ needed
       and count their sums. */
    for (idx_t y = 0; ; ++y) {
        seq_t t = (seq_t)y * (seq_t)(y + 1) / 2;
        if (t > (seq_t)needed)
            break;
        for (idx_t z = 0; ; ++z) {
            seq_t te = (seq_t)z * (seq_t)(z + 1) * (seq_t)(z + 2) / 6;
            if (te > (seq_t)needed)
                break;
            seq_t sum = t + te;
            if (sum > (seq_t)needed)
                break;   /* too large, larger z will only increase sum */
            new_counts[(size_t)sum] += (seq_t)1;
        }
    }

    /* Replace the old table.  The old memory can be freed
       after the replacement to avoid leaks. */
    if (tritetra_counts)
        free(tritetra_counts);
    tritetra_counts = new_counts;
    tritetra_maxsize = needed;
}

/* Main sequence function.  Returns a(n) with 1‑based offset. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but safe return */

    /* Make sure we have counts for all sums up to n. */
    ensure_tritetra_counts(n);

    seq_t result = 0;

    for (idx_t x = 1; ; ++x) {
        seq_t hex = (seq_t)x * (seq_t)(2 * x - 1);
        if (hex > (seq_t)n)
            break;
        result += tritetra_counts[(size_t)(n - hex)];
    }

    return result;
}
