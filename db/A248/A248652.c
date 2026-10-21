/*
 * A248652  Union of the factorial numbers (A000142) and the double factorials
 *          of odd numbers (A001147).
 * https://oeis.org/A248652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth element of the sorted union of * the factorial
 *            numbers (n!) and the odd double factorials ((2k‑1)!!)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    seq_t fact = 1;      /* current factorial value (k!) */
    seq_t doubleVal = 1; /* current odd double factorial value ((2k-1)!!) */
    idx_t fact_i = 1;     /* current k for factorial */
    idx_t double_i = 1;   /* current k for odd double factorial */
    bool fact_exhausted = false;
    bool double_exhausted = false;

    seq_t prev;
    bool first = true;

    while (count < n) {
        /* choose which sequence contributes the next term */
        bool use_fact;
        if (fact_exhausted) {
            use_fact = false;
        } else if (double_exhausted) {
            use_fact = true;
        } else {
            use_fact = (fact <= doubleVal);
        }

        seq_t current = use_fact ? fact : doubleVal;

        if (first || current != prev) {
            ++count;
            if (count == n)
                return current;
            prev = current;
            first = false;
        }

        /* advance the chosen sequence, if possible */
        if (use_fact) {
            ++fact_i;
            seq_t next = fact * fact_i;
            /* detect 64‑bit overflow (no effect for big‑int) */
            if (!fact_exhausted && fact_i > 0 && next / fact_i == fact) {
                fact = next;
            } else {
                fact_exhausted = true;
            }
        } else {
            ++double_i;
            seq_t odd = (seq_t)(2 * double_i - 1);
            seq_t next = doubleVal * odd;
            if (!double_exhausted && odd > 0 && next / odd == doubleVal) {
                doubleVal = next;
            } else {
                double_exhausted = true;
            }
        }
    }

    return (seq_t)0; /* should never be reached */
}
