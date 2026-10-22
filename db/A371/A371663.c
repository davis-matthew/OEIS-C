/*
 * A371663  a(n) is the number of sides of simple polygons (sorted in
 *          ascending order) for which one or more arithmetic sequences can be
 *          formed from all their interior angles (all integer, in degrees).
 * https://oeis.org/A371663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   A(n) is the nth value in the increasing list of n for which a
 *            simple polygon with n sides has interior angles that form an
 *            integer arithmetic progression (angles 1..359 degrees inclusive)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed headers */
#include <stddef.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build environment. */
/*   * seq_t   : integer type for sequence values (possibly big integer)   */
/*   * idx_t   : unsigned integer type for indices (uint64_t)              */

/* Static table to store the first 27 terms of A371663 */
static seq_t a_values[30];
static size_t a_count = 0;
static bool a_ready = false;

/* Helper to precompute the sequence up to 27 terms */
static void compute_sequence(void)
{
    if (a_ready)
        return;
    a_ready = true;
    a_count = 0;

    /* Search for n from 3 upward until we find 27 terms or n exceeds 1000.
       The sequence is finite, and all terms are ≤ 360. */
    for (idx_t n = 3; n <= 1000 && a_count < 27; ++n) {
        seq_t S = (seq_t)180 * (seq_t)(n - 2);            /* S = sum of interior angles */
        seq_t C = (seq_t)n * (seq_t)(n - 1) / (seq_t)2;   /* C = n*(n-1)/2 */

        /* Maximum absolute common difference allowed (angles 1..359) */
        idx_t max_d = (idx_t)(358 / (n - 1));
        bool found = false;

        /* d = 0 case (regular polygon) */
        if (S % (seq_t)n == 0) {
            seq_t a = S / (seq_t)n;
            if (a >= (seq_t)1 && a <= (seq_t)359)
                found = true;
        }

        /* d > 0 cases */
        for (idx_t d = 1; !found && d <= max_d; ++d) {
            seq_t numerator = S - (seq_t)d * C;
            if (numerator % (seq_t)n == 0) {
                seq_t a = numerator / (seq_t)n;
                if (a >= (seq_t)1 &&
                    a + (seq_t)(n - 1) * (seq_t)d <= (seq_t)359) {
                    found = true;
                    break;
                }
            }
        }

        /* d < 0 cases */
        for (idx_t d = 1; !found && d <= max_d; ++d) {
            seq_t numerator = S + (seq_t)d * C;   /* because d is negative here */
            if (numerator % (seq_t)n == 0) {
                seq_t a = numerator / (seq_t)n;
                /* For negative d, last angle = a - (n-1)*d */
                seq_t last = a - (seq_t)(n - 1) * (seq_t)d;
                if (a <= (seq_t)359 && last >= (seq_t)1) {
                    found = true;
                    break;
                }
            }
        }

        if (found)
            a_values[a_count++] = (seq_t)n;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but return 0 */

    compute_sequence();

    if (n <= a_count)
        return a_values[n - 1];
    else
        return (seq_t)0;   /* value beyond the known terms */
}
