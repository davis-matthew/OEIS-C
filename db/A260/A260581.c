/*
 * A260581  Numbers n for which d(n+d(n)) > d(n), where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A260581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   alloc_seq_array, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260581: numbers n such that d(n + d(n)) > d(n).
   The implementation scans integers, tests the condition,
   and stores found terms in a static array so that subsequent
   calls for larger indices can reuse previously computed values. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/alloc_seq_array.h"

static seq_t *terms = NULL;        /* array of found terms */
static const size_t TERMS_CAPACITY = 20000; /* enough for n up to ~10000 */
static size_t terms_len = 0;        /* number of terms stored */
static uint64_t last_checked = 0;   /* last integer inspected */

/* Allocate the static array on first use. */
static void ensure_terms_storage(void)
{
    if (terms != NULL)
        return;
    /* alloc_seq_array returns a properly constructed array of seq_t. */
    terms = alloc_seq_array(TERMS_CAPACITY);
}

/* Return the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n==0 should not occur */
        return 0;

    ensure_terms_storage();

    /* Compute terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        ++last_checked;   /* move to the next integer */

        /* Compute d(last_checked) and d(last_checked + d(last_checked)). */
        uint64_t d_n = divisor_count_u64(last_checked);
        uint64_t m   = last_checked + d_n;
        uint64_t d_m = divisor_count_u64(m);

        if (d_m > d_n) {
            /* Store the term.  The cast is safe for all supported seq_t. */
            terms[terms_len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
